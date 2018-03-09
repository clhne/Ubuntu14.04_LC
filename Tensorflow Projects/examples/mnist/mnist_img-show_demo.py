#-*-coding:utf-8-*-
import matplotlib.pyplot as plt  
from tensorflow.examples.tutorials.mnist import input_data  
import tensorflow as tf  
import numpy as np  
mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)  
#data_trainsform这个函数是将数字转化成可视化的图像  
def data_trainsform(a):    #将784转换成28*28的矩阵  
    b = np.zeros([28, 28]) #定义一个简单的28X28矩阵  
    for i in range(0,27):  
        for j in range(0,27):  
            b[i][j] = a[28*i+j]  
    return b  
  
tile = data_trainsform(mnist.train.images[1])  
print mnist.train.labels[1]  
plt.figure()  
plt.imshow(tile)  
plt.show()  
print mnist.train.images.shape  
print mnist.train.labels.shape  
print mnist.validation.images.shape  
print mnist.validation.labels.shape  
print mnist.test.images.shape  
print mnist.test.labels.shape  
