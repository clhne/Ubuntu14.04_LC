#-*-coding=utf-8-*-
# tf.stack 矩阵拼接,tf.unstack矩阵分解,0为水平分解,1为垂直分解
import tensorflow as tf
a = tf.constant([1,2,3])
b = tf.constant([4,5,6])
c = tf.stack([a,b],axis = 0)
f = tf.stack([a,b],axis = 1)
d = tf.unstack(c,axis = 0)
e = tf.unstack(c,axis = 1)
print c.get_shape()
sess = tf.Session()
print sess.run(f)
print sess.run(c)
print sess.run(d)
print sess.run(e)


