import cv2
import matplotlib.pyplot as plt

img = cv2.imread('/home/lcl/Desktop/projects/python-project/pyramid/rgb.png')
img_temp = cv2.pyrDown(img)
plt.subplot(121),plt.imshow(img,'gray')
plt.subplot(122),plt.imshow(img_temp,'gray')
#plt.subplot(121),plt.imshow('gray',img)
#plt.subplot(122),plt.imshow('gray',img_temp)
