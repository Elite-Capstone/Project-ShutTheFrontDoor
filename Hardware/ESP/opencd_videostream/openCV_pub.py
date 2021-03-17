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

context = zmq.Context()
footage_socket = context.socket(zmq.PUB)

# camera server '192.168.1.19:80/stream'
cam_ip = "192.168.1.19"
cam_port = "80"
cam_uri = "stream"
# Publisher server 'tcp://localhost:5555'
ip = 'localhost'
port = 5555

target_address = "tcp://{}:{}".format(ip, port)
print("Publish Video to ", target_address)
footage_socket.connect("tcp://localhost:%s" % port)

camera = cv2.VideoCapture("http://{}:{}/{}".format(cam_ip, cam_port, cam_uri))

camera.set(cv2.CAP_PROP_FRAME_WIDTH, 960)
camera.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
print("Start Time: ", datetime.datetime.now())
fps = FPS().start()
while True:
        try:
            (grabbed, frame) = camera.read()  # grab the current frame
            print(grabbed, frame)
            frame = cv2.resize(frame, (960, 720))  # resize the frame
            frame = cv2.cv2.rotate(frame, cv2.cv2.ROTATE_90_CLOCKWISE)
            encoded, buffer = cv2.imencode('.jpg', frame)
            footage_socket.send(base64.b64encode(buffer))
            # Update the FPS counter
            fps.update()
            # cv2.imshow('PUB', frame)
            # cv2.waitKey(1)
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


