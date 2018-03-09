import cv2 
img = cv2.imread('dp.bmp')
img = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
cv2.filterSpeckles(img,0,800,0)

cv2.namedWindow('image')
cv2.imshow('image',img)
cv2.waitKey(0)
cv2.destroyAllWindows()
