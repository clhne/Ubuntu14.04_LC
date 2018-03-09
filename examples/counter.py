import tensorflow as tf
state  = tf.Variable(0,name = "counter")

one = tf.constant(1)
new_value = tf.add(state,one)
update = tf.assign(state,new_value)

init_op = tf.global_variables_initializer()

#with tf.Session() as sess:
sess = tf.Session()
sess.run(init_op)
print sess.run(state)

for _ in range(10):
    sess.run(update)
    print sess.run(state)	
