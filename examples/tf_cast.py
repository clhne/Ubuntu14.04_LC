#-*-coding:utf-8-*-
#tf.cast()数据转换
import tensorflow as tf 
a = tf.Variable([1,0,0,1,1],dtype = tf.float32)
b = tf.cast(a,dtype = tf.bool)
c = tf.reduce_mean(a)

sess = tf.Session()
sess.run(tf.global_variables_initializer())

print '\n'
print sess.run(b)
print '\n'
print sess.run(c)
