import tensorflow as tf
# define objectfunction 
x = tf.Variable(tf.truncated_normal([1]),name = "x")
goal = tf.pow(x-3,2,name = "goal")

with tf.Session() as sess:
	x.initializer.run()
	print x.eval()
	print goal.eval()
	
# minimize()
optimizer = tf.train.GradientDescentOptimizer(learning_rate = 0.17)
train_step = optimizer.minimize(goal)

def train():
	with tf.Session() as sess:
		x.initializer.run()
		for i in range(1000):
			print "x: ", x.eval()
			train_step.run()
			print "goal: ",goal.eval()
train()

y = tf.Variable(tf.truncated_normal([1]))
max_goal = tf.sin(y)
optimizer = tf.train.GradientDescentOptimizer(learning_rate = 0.17)
train_step = optimizer.minimize(-1 * max_goal)

with tf.Session() as sess:
	y.initializer.run()
	for i in range(1000):
		print "y: ", y.eval()
		train_step.run()
		print "max_goal: ",max_goal.eval()
		
gra_and_var = optimizer.compute_gradients(goal)
train_step = optimizer.apply_gradients(gra_and_var)
train()

# clip_by_global_norm
gradients, vriables = zip(*optimizer.compute_gradients(goal))
gradients, _ = tf.clip_by_global_norm(gradients,1.255)
train_step = optimizer.apply_gradients(zip(gradients,vriables))
train()

# exponential_decay
# global_step 
global_step = tf.Variable(0)
learning_rate = tf.train.exponential_decay(3.0,global_step,3,0.3,staircase=True)
optimizer2 = tf.train.GradientDescentOptimizer(learning_rate)
gradients, vriables = zip(*optimizer2.compute_gradients(goal))
gradients, _ = tf.clip_by_global_norm(gradients,1.255)
train_step = optimizer2.apply_gradients(zip(gradients,vriables),global_step = global_step)

with tf.Session() as sess:
		global_step.initializer.run()
		x.initializer.run()
		for i in range(1000):
			print "x: ", x.eval()
			train_step.run()
			print "goal: ",goal.eval()
		
