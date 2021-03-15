import os, sys, datetime
import json, base64
import cv2
import zmq
import numpy as np
import imutils
from imutils.video import FPS

##pip install opencv-python
##pip install pyzmq
##pip install imutils

### run with python openCD.py pub --192.168.1.19/stream --pubport=80


def pubVideo(config):
    context = zmq.Context()
    footage_socket = context.socket(zmq.PUB)
    # 'tcp://localhost:5555'
    ip = "192.168.1.19"
    port = 5555
    target_address = "tcp://{}:{}".format(ip, port)
    print("Publish Video to ", target_address)
    footage_socket.connect(target_address)
    camera = cv2.VideoCapture('http://192.168.1.19:80/stream')
    camera.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    camera.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
    print("Start Time: ", datetime.datetime.now())
    fps = FPS().start()
    while True:
        try:
            (grabbed, frame) = camera.read()
            frame = cv2.resize(frame, (640, 480)) 
            encoded, buffer = cv2.imencode('.jpg', frame)
            footage_socket.send_string(base64.b64encode(buffer))
            # Update the FPS counter
            fps.update()
            cv2.waitKey(1)
        except KeyboardInterrupt:
            # stop the timer and display FPS information
            fps.stop()
            print("[INFO] elasped time: {:.2f}".format(fps.elapsed()))
            print("[INFO] approx. FPS: {:.2f}".format(fps.fps()))
            camera.release()
            cv2.destroyAllWindows()
            print("\n\nBye bye\n")
            break
    print("End Time: ", datetime.datetime.now())


#sub with python CvZmq.py sub --subport=5555

def subVideo(config):
    context = zmq.Context()
    footage_socket = context.socket(zmq.SUB)
    port = 5555
    bind_address = "tcp://192.168.1.19:{}".format(port)  # 'tcp://*:5555'
    print("Subscribe Video at ", bind_address)
    footage_socket.bind(bind_address)
    footage_socket.setsockopt_string(zmq.SUBSCRIBE, unicode(''))
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
