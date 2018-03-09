import tensorflow as tf 
sess = tf.InteractiveSession()
x = tf.Variable([1.,2.])
a = tf.constant([3.,3.])
x.initializer.run()
sub = tf.subtract(x,a)
print sub.eval()
