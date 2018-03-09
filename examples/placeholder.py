#-*-coding:utf-8-*-
import tensorflow as tf 
#placeholder描述等待输入的节点，要和feed_dict绑定使用，feed调用placeholder方法
input1 = tf.placeholder(tf.float32)
input2 = tf.placeholder(tf.float32)
output = tf.multiply(input1,input2)

sess = tf.Session()
print '\n' 
print sess.run(output,feed_dict = {input1:[7.],input2:[5.]})

