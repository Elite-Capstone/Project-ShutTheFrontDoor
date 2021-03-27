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

SHUTDOWN_STR = "Shutting down socket..."

#==================================================================
# This code is to use for the a redirecting server, that will 
# receiving the stream and resend it to the client
#==================================================================

# -----------  Config  ----------
HOST = socket.gethostbyname(socket.gethostname())
HOST_PORT = 5555
BUFSIZE = 64000  # Maximum buffer size for receiving
ADDR = (HOST, HOST_PORT)
print(ADDR)
#-----  Stream Client Config  -----
# If client is computer, maybe use router IP
CLIENT = ''
CLIENT_PORT = 5555

    
class UdpServer:

    def __init__(self, host, host_port, client, client_port, family_addr, persist=False):
        self.host = host
        self.host_port = host_port
        self.family_addr = family_addr
        self.socket = socket.socket(family_addr, socket.SOCK_DGRAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

        self.client = client
        self.client_port = client_port

        self.shutdown = Event()
        self.persist = persist

        try:
            self.socket.bind(self.host, self.port)
        except socket.error as e:
            print("Bind failed:{}".format(e))
            raise

        self.server_thread = Thread(target=self.run_server)
        self.server_thread.start()

    def stop_server(self):
        print("Shutdown socket manually from device")
        self.socket.sendto(SHUTDOWN_STR, CLIENT)
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
                data, addr = self.socket.recvfrom(BUFSIZE)
                if not data:
                    return
                if data.decode() == SHUTDOWN_STR:
                    # Received Shutdown command. Pass command to receiving stream client
                    stop_server(self)
                    return

                if data != b"":
                    # print(data, addr)
                    self.socket.sendto(data, CLIENT)

            except socket.error as e:
                print("Running server failed:{}".format(e))
                raise
            if not self.persist:
                break
        

if __name__ == '__main__':
    family_addr = socket.AF_INET
    UdpServer(HOST, HOST_PORT, CLIENT, CLIENT_PORT, family_addr, persist=True)