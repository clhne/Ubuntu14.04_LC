import cv2
img = cv2.imread('dp.bmp')
cv2.namedWindow("image")

cv2.imshow("image",img)
cv2.waitKey(0)
cv2.destroyAllWindows()
