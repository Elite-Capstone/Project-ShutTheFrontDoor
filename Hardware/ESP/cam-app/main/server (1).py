import cv2
import socket
import pickle
import numpy as np

host = "127.0.0.1"
port = 5555
max_length = 64000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# sock.setsockopt(socket.SOL_SOCKET,socket.SO_RCVBUF, 1)
sock.bind(('', port))

frame_info = None
buffer = None
frame = None

print("-> waiting for connection")

while True:
    data, address = sock.recvfrom(max_length)
    
    if len(data) < 100:
        print("Got stuff")
        frame_info = pickle.loads(data)

        if frame_info:
            nums_of_packs = frame_info["packs"]

            # send the number of packs to be expected
            print("Number of packs:", nums_of_packs)


            for i in range(nums_of_packs):
                data, address = sock.recvfrom(max_length)

                if i == 0:
                    buffer = data
                else:
                    buffer += data

            frame = np.frombuffer(buffer, dtype=np.uint8)
            frame = frame.reshape(frame.shape[0], 1)

            frame = cv2.imdecode(frame, cv2.IMREAD_COLOR)
            frame = cv2.flip(frame, 1)
            
            if frame is not None and type(frame) == np.ndarray:
                cv2.imshow("Stream", frame)
                if cv2.waitKey(1) == 27:
                    break
    else:
        print("lenght is longer than 100")
                
print("goodbye")
