import cv2
import numpy as np

detector = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

img = cv2.imread("imggroup.jpg")
#cv2.imshow('pic original',img)
#convert img to Gray scale
gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
faces = detector.detectMultiScale(gray,1.3,5)
for (x,y,w,h) in faces:
    cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)
cv2.imwrite('detect_result.jpg',img)




