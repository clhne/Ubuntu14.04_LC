import tensorflow as tf
bs_graph = tf.Graph()
with bs_graph.as_default():
    x = tf.constant(2.0,name = 'input-x')
    y = tf.constant(0.9,name = 'input-y')
    z = tf.add(x,y,name = 'output_z')
    bs_init = tf.global_variables_initializer()

with tf.Session(graph=bs_graph) as bs_sess:
    bs_sess.run(bs_init)
    print(bs_sess.run(z))
    tf.summary.FileWriter('log',bs_sess.graph)
#tensorboard --logdir=log

