#coding=utf-8
import tensorflow as tf
#"Hello, Tensorflow"是一个0维的字符串，即tensor(张量)
hello = tf.constant('Hello, Tensorflow!')
# Session会话，将操作分配给CPU/GPU,包括远程机器,用tf.Session()创建会话实例sess
sess = tf.Session()
# sess.run()运行定义的函数计算tensor，并返回结果
output1 = sess.run(hello)
print
print(output1)

a = tf.constant(10)
b = tf.constant(32)
output2 = sess.run(a+b)
print
print(output2)

