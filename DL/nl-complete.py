#-*-coding:utf-8-*-
import time
from numpy import exp, array, random, dot
start = time.time()
class neuralnetwork():
    def __init__(self):
	#随机数种子，每次获得相同结果
        random.seed(1)
	#神经元建模，赋予随机权重，（－1，1）
        self.synaptic_weights = 2 * random.random((3,1)) - 1
    #Sigmoid function, (0,1)
    def __sigmoid(self,x):
        return 1 / (1 + exp(-x))
    #Sigmoid 求导，梯度
    def __sigmoid_derivative(self,x):
        return x * (1 - x)
    def train(self,training_set_inputs,training_set_outputs,number_of_training_iterations):
        for iteration in xrange(number_of_training_iterations):
            output=self.think(training_set_inputs)
            error=training_set_outputs - output
            adjustment=dot(training_set_inputs.T,error*self.__sigmoid_derivative(output))
            self.synaptic_weights += adjustment
    def think(self,inputs):
         return self.__sigmoid(dot(inputs,self.synaptic_weights))

if __name__ == "__main__":
    neural_network=neuralnetwork()
    print "随机初始突触权重:"
    print neural_network.synaptic_weights
    training_set_inputs=array([[0,0,1],[1,1,1],[1,0,1],[0,1,1,]])
    training_set_outputs=array([[0,1,1,0]]).T
    #neural_network.train(training_set_inputs,training_set_outputs,200000)
    neural_network.train(training_set_inputs,training_set_outputs,20000)
    print "训练后的突触权重："
    print neural_network.synaptic_weights

    print "预测 [1,0,0] -> ?"
    print neural_network.think(array([1,0,0]))

    end = time.time()
    print "Run time: "
    print end - start
