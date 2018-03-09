#-*-coding:utf-8-*-
import tensorflow as tf
import input_data
import time
start=time.time()
#读取MNIST数据集
mnist=input_data.read_data_sets("MNIST_data/",one_hot=True)
#利用占位符预定义x
x = tf.placeholder(tf.float32,[None, 784])
#初始化w和b
w = tf.Variable(tf.zeros([784,10]))
b = tf.Variable(tf.zeros([10]))
#y预测值,softmax model
y = tf.nn.softmax(tf.matmul(x,w)+b)
#y_真实值
y_=tf.placeholder("float",[None,10])
#交叉熵损失函数
cross_entropy=-tf.reduce_sum(y_*tf.log(y))
#用梯度下降法优化
train_step=tf.train.GradientDescentOptimizer(0.01).minimize(cross_entropy)
#初始化模型的所有变量
#init = tf.initialize_all_variables() 2017.03.02之后些方法废弃
init = tf.global_variables_initializer()
#启动模型并初始化
sess=tf.Session()
sess.run(init)
#训练4000次，每次抽取100个批数据
for i in range(4000):
    batch_xs,batch_ys=mnist.train.next_batch(100)
    sess.run(train_step,feed_dict={x:batch_xs,y_:batch_ys})
#预测值和真实值
correct_prediction=tf.equal(tf.argmax(y,1),tf.argmax(y_,1))
accuracy=tf.reduce_mean(tf.cast(correct_prediction,"float"))
end=time.time()
t=end-start
print "run time: "
print t
print "accuracy: "
print sess.run(accuracy,feed_dict={x:mnist.test.images,y_:mnist.test.labels})
