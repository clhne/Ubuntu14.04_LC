#-*-coding:utf-8-*-

import tensorflow as tf
import input_data
import matplotlib.pyplot as plt
import numpy as np
import random as ran
mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)

def TRAIN_SIZE(num):

    print "Total Training Images in Dataset = " + str(mnist.train.images.shape)
    print "————————————————–"

    x_train = mnist.train.images[:num,:]
    print "x_train Examples Loaded = " + str(x_train.shape)

    y_train = mnist.train.labels[:num,:]
    print "y_train Examples Loaded = " + str(y_train.shape)
    print ""
    return x_train, y_train

def TEST_SIZE(num):

    print "Total Test Examples in Dataset = " + str(mnist.test.images.shape)
    print "————————————————–"

    x_test = mnist.test.images[:num,:]
    print "x_test Examples Loaded = " + str(x_test.shape)
    
    y_test = mnist.test.labels[:num,:]
    print "y_test Examples Loaded = " + str(y_test.shape)
    return x_test, y_test


def display_digit(num):
    print y_train[num]
    label = y_train[num].argmax(axis=0)
    image = x_train[num].reshape([28,28])

    plt.title("Example: %d  Label: %d" % (num, label))
    plt.imshow(image, cmap=plt.get_cmap("gray_r"))
    plt.show()

def display_mult_flat(start, stop):
    images = x_train[start].reshape([1,84])
    for i in range(start+1,stop):
        images = np.concatenate((images, x_train[i].reshape([1,84])))

    plt.imshow(images, cmap=plt.get_cmap("gray_r"))
    plt.show()

    x_train, y_train = TRAIN_SIZE(5500)
#Total Training Images in Dataset = (55000, 784)
#x_train Examples Loaded = (55000, 784)
#y_train Examples Loaded = (55000, 10)

    display_digit(ran.randint(0, x_train.shape[0]))

    display_mult_flat(0,400)

sess = tf.Session()

x = tf.placeholder(tf.float32,shape=[None,84])
y_ = tf.placeholder(tf.float32, shape=[None, 10])
W = tf.Variable(tf.zeros([84,10]))
b = tf.Variable(tf.zeros([10]))
y = tf.nn.softmax(tf.matmul(x,W) + b)
print y

#Tensor("Softmax:0", shape=(?, 10), dtype=float32)

x_train, y_train = TRAIN_SIZE(3)

sess.run(tf.global_variables_initializer())

print sess.run(y, feed_dict={x: x_train})
sess.run(tf.nn.softmax(tf.zeros([4])))

sess.run(tf.nn.softmax(tf.constant([0.1, 0.005, 2])))
cross_entropy = tf.reduce_mean(-tf.reduce_sum(y_ * tf.log(y), reduction_indices=[1]))

for i in range(10):
    plt.subplot(2, 5, i+1)

weight = sess.run(W)[:,i]

plt.title(i)

plt.imshow(weight.reshape([28,28]), cmap=plt.get_cmap("seismic"))

frame1 = plt.gca()

frame1.axes.get_xaxis().set_visible(False)

frame1.axes.get_yaxis().set_visible(False)
plt.show()

x_train, y_train = TRAIN_SIZE(1)

display_digit(0)
answer = sess.run(y, feed_dict={x: x_train})

print answer
answer.argmax()

def display_compare(num):

# THIS WILL LOAD ONE TRAINING EXAMPLE
    x_train = mnist.train.images[num,:].reshape(1,84)
    y_train = mnist.train.labels[num,:]

# THIS GETS OUR LABEL AS A INTEGER

label = y_train.argmax()

# THIS GETS OUR PREDICTION AS A INTEGER

prediction = sess.run(y, feed_dict={x: x_train}).argmax()

plt.title("Prediction: %d Label: %d" % (prediction, label))

plt.imshow(x_train.reshape([28,28]), cmap=plt.get_cmap("gray_r"))

plt.show()


display_compare(ran.randint(0, 5500))
