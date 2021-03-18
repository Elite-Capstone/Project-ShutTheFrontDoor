from __future__ import print_function
from __future__ import unicode_literals
from builtins import input
import os
import re
import socket
from threading import Thread, Event
import sys

import cv2
import zmq
import base64
import numpy
from numpy.compat import unicode

# stream_socket = zmq.Context().sokect(zmq.SUB)
# stream_socket.bind('tcp://*:80')
# stream_socket.setsockopt_string(zmq.SUBSCRIBE, unicode(''))

# while True:

#     img_enc = stream_socket.recv_string()
#     img_string = base64.b64decode(img_enc)
#     npimg = numpy.fromstring(img_string, dtype=numpy.uint8)

#     source = cv2.imdecode(npimg, 1)
#     source = cv2.rotate(source, cv2.cv2.ROTATE_90_CLOCKWISE)
#     cv2.imshow('IP Camera stream',source)

#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         break

# cv2.destroyAllWindows()

# -----------  Config  ----------
HOST = ''
PORT = 5555
BUFSIZE = 1024  # Maximum buffer size for receiving
ADDR = (HOST, PORT)
print(ADDR)

# ESP_IP = "192.168.1.1"
# ESP_PORT = 13585
# ESP_ADDR = (ESP_IP, ESP_PORT)

client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
client.bind(ADDR)
# -------------------------------

# def get_my_ip(type):
#     for i in netifaces.ifaddresses(INTERFACE)[type]:
#         return i['addr'].replace("%{}".format(INTERFACE), "")

def receive():
    print("waiting for receiving from UDP port...")
    data, addr = client.recvfrom(BUFSIZE)
    if data != b"":
        print(data, addr)
        npimg = numpy.fromstring(data, dtype=numpy.uint8)
        source = cv2.imdecode(npimg, 1)
        source = cv2.rotate(source, cv2.cv2.ROTATE_90_CLOCKWISE)
        cv2.imshow('IP Camera stream',source)
    

class UdpServer:

    def __init__(self, port, family_addr, persist=False):
        self.port = port
        self.family_addr = family_addr
        self.socket = socket.socket(family_addr, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

        self.socket.settimeout(30.0)
        self.shutdown = Event()
        self.persist = persist

    def __enter__(self):
        try:
            self.socket.bind(('', self.port))
        except socket.error as e:
            print("Bind failed:{}".format(e))
            raise

        self.server_thread = Thread(target=self.run_server)
        self.server_thread.start()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        if self.persist:
            sock = socket.socket(self.family_addr, socket.SOCK_DGRAM)
            sock.sendto(b'Stop', ('localhost', self.port))
            sock.close()
            self.shutdown.set()
        self.server_thread.join()
        self.socket.close()

    def run_server(self):
        while not self.shutdown.is_set():
            try:
                data, addr = self.socket.recvfrom(1024)
                if not data:
                    return
                data = data.decode()
                print('Reply[' + addr[0] + ':' + str(addr[1]) + '] - ' + data)
            except socket.error as e:
                print("Running server failed:{}".format(e))
                raise
            if not self.persist:
                break

if __name__ == '__main__':
    #if sys.argv[1:] and sys.argv[1].startswith("IPv"):     # if additional arguments provided:
        # Usage: example_test.py <IPv4|IPv6>
        # family_addr = socket.AF_INET6 if sys.argv[1] == "IPv6" else socket.AF_INET
    family_addr = socket.AF_INET
    with UdpServer(PORT, family_addr, persist=True) as s:
        print(input("Press Enter stop the server..."))
    # else:
    #     test_examples_protocol_socket()

# @ttfw_idf.idf_example_test(env_tag="Example_WIFI")
# def test_examples_protocol_socket(env, extra_data):
#     """
#     steps:
#       1. join AP
#       2. have the board connect to the server
#       3. send and receive data
#     """
#     dut1 = env.get_dut("udp_client", "examples/protocols/sockets/udp_client", dut_class=ttfw_idf.ESP32DUT)
#     # check and log bin size
#     binary_file = os.path.join(dut1.app.binary_path, "udp_client.bin")
#     bin_size = os.path.getsize(binary_file)
#     ttfw_idf.log_performance("udp_client_bin_size", "{}KB".format(bin_size // 1024))
#     ttfw_idf.check_performance("udp_client_bin_size", bin_size // 1024, dut1.TARGET)

#     # start test
#     dut1.start_app()

#     data = dut1.expect(re.compile(r" IPv4 address: ([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)"), timeout=30)
#     print("Connected with IPv4: {}".format(data[0]))

#     # test IPv4
#     with UdpServer(PORT, socket.AF_INET):
#         dut1.write(get_my_ip(netifaces.AF_INET))
#         dut1.expect(re.compile(r"OK: Message from ESP32"))
#     # test IPv6
#     with UdpServer(PORT, socket.AF_INET6):
#         dut1.write(get_my_ip(netifaces.AF_INET6))
#         dut1.expect(re.compile(r"OK: Message from ESP32"))

# Use the next line if your camera has a username and password
# stream = cv2.VideoCapture('protocol://username:password@IP:port/1')
# if stream.isOpened() == False:
#     stream.open()

# while True:

#     ret, frame = stream.read()
#     #frame = cv2.rotate(frame, cv2.cv2.ROTATE_90_CLOCKWISE)
#     cv2.imshow('IP Camera stream',frame)

#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         break

# cv2.destroyAllWindows()
