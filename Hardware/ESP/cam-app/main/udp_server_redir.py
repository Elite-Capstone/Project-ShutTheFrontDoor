from __future__ import print_function
from __future__ import unicode_literals

import base64
import datetime
import os
import re
import socket
from threading import Thread, Event

import numpy
import zmq
from imutils.video import FPS

SHUTDOWN_STR = "Shutting down socket..."

# ==================================================================
# This code is to use for the a redirecting server, that will 
# receiving the stream and resend it to the client
# ==================================================================

# -----------  Config  ----------
HOST = socket.gethostbyname(socket.gethostname())
HOST_PORT = 5555
BUFSIZE = 64000  # Maximum buffer size for receiving
ADDR = (HOST, HOST_PORT)
print(ADDR)
# -----  Stream Client Config  -----
# If client is computer, maybe use router IP
CLIENT = '65.97.41.217'
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

        self.context = zmq.Context()
        self.footage_socket = self.context.socket(zmq.PUB)
        self.fps = FPS().start()

        try:
            print("{}:{}".format(self.host, self.host_port))
            self.socket.bind((self.host, self.host_port))
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
                # if data.decode() == SHUTDOWN_STR:
                #     # Received Shutdown command. Pass command to receiving stream client
                #     self.stop_server(self)
                #     return

                if data != b"":
                    # print(data, addr)
                    self.socket.sendto(data, (CLIENT, CLIENT_PORT))
                    # self.zmq_pub(data)

            except socket.error as e:
                print("Running server failed:{}".format(e))
                raise
            if not self.persist:
                break

    def zmq_pub(self, data):
        try:
            npimg = numpy.frombuffer(data, dtype=numpy.uint8)
            print(npimg)
            self.footage_socket.send(base64.b64encode(npimg))
            # Update the FPS counter
            self.fps.update()
        except KeyboardInterrupt:
            # stop the timer and display FPS information
            self.fps.stop()
            print("[INFO] elasped time: {:.2f}".format(self.fps.elapsed()))
            print("[INFO] approx. FPS: {:.2f}".format(self.fps.fps()))
            print("\n\nBye bye\n")


if __name__ == '__main__':
    family_addr = socket.AF_INET
    UdpServer(HOST, HOST_PORT, CLIENT, CLIENT_PORT, family_addr, persist=True)
