from __future__ import print_function
from __future__ import unicode_literals
from builtins import input
import os
import re
import socket
from threading import Thread, Event
import sys

import cv2
import self as self
import zmq
import base64
import numpy
from numpy.compat import unicode

# -----------------------------------------------------------
# Code used to distribute video with ZMQ
#
# stream_socket = zmq.Context().sokect(zmq.SUB)
# stream_socket.bind('tcp://*:80')
# stream_socket.setsockopt_string(zmq.SUBSCRIBE, unicode(''))
#
# while True:
#
#     img_enc = stream_socket.recv_string()
#     img_string = base64.b64decode(img_enc)
#     npimg = numpy.fromstring(img_string, dtype=numpy.uint8)
#
#     source = cv2.imdecode(npimg, 1)
#     source = cv2.rotate(source, cv2.cv2.ROTATE_90_CLOCKWISE)
#     cv2.imshow('IP Camera stream',source)
#
#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         break
#
# cv2.destroyAllWindows()
# -------------------------------------------------------------------

# ==================================================================
# This code is to use for the receiving server that will display the stream
# ==================================================================

# -----------  Config  ----------

HOST = ""  # IP of Ben's local machine: '192.168.1.17'
PORT = 5555
BUFSIZE = 64000  # Maximum buffer size for receiving
ADDR = (HOST, PORT)
print(ADDR)


class UdpServer:

    def __init__(self, host, port, family_addr, persist=False):
        self.host = host
        self.port = port
        self.family_addr = family_addr
        self.socket = socket.socket(family_addr, socket.SOCK_DGRAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

        # self.socket.settimeout(30.0) # Sets the interval of time after which if there is no event, the server shuts down
        self.shutdown = Event()
        self.persist = persist

    def __enter__(self):
        try:
            # Binds to an address that is accessible by the device
            # If the address is not an interface of the device it will not see the IP
            self.socket.bind((self.host, self.port))
        except socket.error as e:
            print("Bind failed:{}".format(e))
            raise

        self.server_thread = Thread(target=self.run_server)
        self.server_thread.start()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        if self.persist:
            sock = socket.socket(self.family_addr, socket.SOCK_DGRAM)
            sock.sendto(b'Stop', ('', 5555))
            sock.close()
            cv2.destroyAllWindows()
            self.shutdown.set()
        self.server_thread.join()
        self.socket.close()

    def run_server(self):
        while not self.shutdown.is_set():
            try:
                data, addr = self.socket.recvfrom(BUFSIZE)
                if not data:
                    return
                # if data.decode() == "Shutting down socket...":
                #     print("Shutdown socket manually from device")
                #     return
                if data != b"":
                    # print(data, addr) # For debug
                    npimg = numpy.frombuffer(data, dtype=numpy.uint8)
                    source = cv2.imdecode(npimg, 1)
                    source = cv2.rotate(source, cv2.cv2.ROTATE_180)
                    cv2.imshow('IP Camera stream', source)
                    if cv2.waitKey(1) & 0xFF == ord('q'):
                        break

            except socket.error as e:
                print("Running server failed:{}".format(e))
                raise
            if not self.persist:
                break


def loop_on_socket(sm):
    while True:
        d, addr = s.recvfrom(1500)
        print(d, addr)
        sm.sendto("ECHO: ".encode('utf8') + d, addr)


# Main function
if __name__ == '__main__':
    family_addr = socket.AF_INET
    with UdpServer(HOST, PORT, family_addr, persist=True) as s:
        print(input("Press Enter stop the server..."))
