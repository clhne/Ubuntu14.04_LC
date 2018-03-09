#-*-coding:utf-8-*-
import tensorflow as tf 
#建图
matrix1 = tf.constant([[3.,3.]])
matrix2 = tf.constant([[2.],[2.]])

product = tf.matmul(matrix1,matrix2)
#启动图
sess = tf.Session()
#取值
result = sess.run(product)
print result
#关闭图
sess.close()

