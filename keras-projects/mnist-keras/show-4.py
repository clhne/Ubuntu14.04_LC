#-*-coding:utf-8-*-
# Author : Lcl
# plot mnist top 4 figure
from keras.datasets import mnist
import matplotlib.pyplot as plt
# load the mnist dataset
(x_train, y_train),(x_test, y_test) = mnist.load_data()
# plot top 4 images as gray scale
plt.subplot(221)
plt.imshow(x_train[0], cmap=plt.get_cmap('PuBuGn_r'))
plt.subplot(222)
plt.imshow(x_train[1], cmap=plt.get_cmap('PuBuGn_r'))
plt.subplot(223)
plt.imshow(x_train[2], cmap=plt.get_cmap('PuBuGn_r'))
plt.subplot(224)
plt.imshow(x_train[3], cmap=plt.get_cmap('PuBuGn_r'))
# show the plot
plt.show()