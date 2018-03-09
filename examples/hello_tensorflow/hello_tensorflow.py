import subprocess
import tensorflow as tf 
import os

#subprocess.call(["source","/tool/tensorlow/bin/activate"],shell = True)
#os.system('. ./tool/tensorflow/bin/activate')


hello = tf.constant('Hello, Tensorflow!')
sess = tf.Session()
result1 = sess.run(hello)
print
print result1 
print

a = tf.constant(10)
b = tf.constant(32)
result2 = sess.run(a + b)
print result2 

sess.close()


