import cv2

stream = cv2.VideoCapture('http://192.168.1.19:80/stream')

# stream = cv2.VideoCapture('protocol://username:password@IP:port/1')

while True:

    r, f = stream.read()
    cv2.imshow('IP Camera stream', f)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()


