#-*-coding:utf-8-*-
import tensorflow as tf

# tf.argmax(input,axis=None,name=None,dimension=None)按行或列计算最大值,axis=0,按列，1按行
a = tf.get_variable(name='a',shape=[3,4],dtype=tf.float32,initializer=tf.random_uniform_initializer(minval=-1,maxval=1))
b = tf.argmax(input=a,axis=0)
c = tf.argmax(input=a,dimension=1)
mat = tf.Variable([[2,5],[7,3],[6,8]])
d = tf.argmax(mat,1)
sess = tf.Session()
#sess = tf.InteractiveSession()
sess.run(tf.global_variables_initializer())
print sess.run(a)
print '\n'
print sess.run(b)
print sess.run(c)
print '\n'
print sess.run(mat)
print '\n'
print sess.run(d)
