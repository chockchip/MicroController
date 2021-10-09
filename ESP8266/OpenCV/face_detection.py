import urllib.request
import cv2
import numpy as np
import time

url='http://192.168.1.47/cam-hi.jpg'

# Load the cascade
face_cascade = cv2.CascadeClassifier('face_detector.xml')

while True:
    print('strat')
    imgResp=urllib.request.urlopen(url)
    imgNp=np.array(bytearray(imgResp.read()),dtype=np.uint8)
    img=cv2.imdecode(imgNp,-1)

    # all the opencv processing is done here


    # Face detection 1
    faces = face_cascade.detectMultiScale(img, 1.1, 4)
    for (x, y, w, h) in faces: 
        cv2.rectangle(img, (x, y), (x+w, y+h), (255, 0, 0), 2)

    cv2.imshow('test',img)
    if ord('q')==cv2.waitKey(10):
        exit(0)
