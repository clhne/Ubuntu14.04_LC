import cv2
img = cv2.imread("tang.jpg")

cv2.imshow("Tang",img)

cv2.waitKey(3000)
cv2.destroyAllWindows()
