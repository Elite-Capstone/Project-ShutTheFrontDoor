import os, sys, datetime
import json, base64
import cv2
import zmq
import numpy as np
import imutils
from imutils.video import FPS
#sub with python CvZmq.py sub --subport=5555

def subVideo(config):
    context = zmq.Context()
    footage_socket = context.socket(zmq.SUB)
    port = 5555
    bind_address = "tcp://192.168.1.19:{}".format(port)  # 'tcp://*:5555'
    print("Subscribe Video at ", bind_address)
    footage_socket.bind(bind_address)
    footage_socket.setsockopt_string(zmq.SUBSCRIBE, str(''))
    while True:
        try:
            frame = footage_socket.recv_string()
            img = base64.b64decode(frame)
            npimg = np.fromstring(img, dtype=np.uint8)
            source = cv2.imdecode(npimg, 1)
            cv2.imshow("image", source)
            cv2.waitKey(1)
        except KeyboardInterrupt:
            cv2.destroyAllWindows()
            print("\n\nBye bye\n")
            break