import os, sys, datetime
import json, base64
import cv2
import zmq
import numpy as np
from numpy.compat import unicode
import imutils
from imutils.video import FPS

# sub with python CvZmq.py sub --subport=5555

context = zmq.Context()
footage_socket = context.socket(zmq.SUB)
port = '5555'
bind_address = 'tcp://*:5555' # 'tcp://192.168.1.19:{}'.format(port)  # 'tcp://*:5555'
print("Subscribe Video at ", bind_address)
# footage_socket.connect ("tcp://localhost:%s" % port)
footage_socket.bind("tcp://*:%s" % port)
footage_socket.setsockopt_string(zmq.SUBSCRIBE, unicode(''))

keep_running = True

while footage_socket and keep_running:
    print(footage_socket)
    try:
        frame = footage_socket.recv_string()
        npimg = np.frombuffer(base64.b64decode(frame), dtype=np.uint8)
        source = cv2.imdecode(npimg, cv2.IMREAD_UNCHANGED)
        cv2.imshow("image", source)
        cv2.waitKey(1)
    except KeyboardInterrupt:
        cv2.destroyAllWindows()
        print("\n\nBye bye\n")
        break

