import cv2
img = cv2.imread('result.jpg')
res = cv2.resize(img,(782,570),interpolation = cv2.INTER_LINEAR)
cv2.imwrite('resize_res.jpg',res)
