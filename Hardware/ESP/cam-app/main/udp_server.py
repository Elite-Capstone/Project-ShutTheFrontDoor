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

#-----------------------------------------------------------
# Code used to distribute video with ZMQ
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
#-------------------------------------------------------------------


# -----------  Config  ----------
HOST = '127.0.0.1'
PORT = 5555
BUFSIZE = 64000  # Maximum buffer size for receiving
ADDR = (HOST, PORT)
print(ADDR)
    
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
                print("waiting to receive from UDP port...")
                data, addr = self.socket.recvfrom(BUFSIZE)
                if data != b"":
                    print(data, addr)
                    npimg = numpy.fromstring(data, dtype=numpy.uint8)
                    source = cv2.imdecode(npimg, 1)
                    source = cv2.rotate(source, cv2.cv2.ROTATE_90_CLOCKWISE)
                    cv2.imshow('IP Camera stream',source)

            except socket.error as e:
                print("Running server failed:{}".format(e))
                raise
            if not self.persist:
                break
        

if __name__ == '__main__':
    family_addr = socket.AF_INET
    with UdpServer(PORT, family_addr, persist=True) as s:
        print(input("Press Enter stop the server..."))