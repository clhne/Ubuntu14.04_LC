#-*-coding:utf-8-*-
import tensorflow as tf

# tf.equal()判断两个矩阵或向量是否相等，返回True,False
a = tf.Variable([1,3,4,5,6])
b = tf.Variable([1,3,4,3,2])

init = tf.global_variables_initializer()

with tf.Session() as sess:
    sess.run(init)
    print sess.run(tf.equal(a,b))


