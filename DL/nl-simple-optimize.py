#-*-coding:utf-8-*-
import time
start=time.time()
from numpy import exp,array,random,dot
training_set_inputs = array([[0,0,1],[1,1,1],[1,0,1],[0,1,1]])

training_set_outputs = array([[0,1,1,0]]).T
random.seed(1)
synaptic_weights = 2 * random.random((3,1)) - 1
#迭代次数为200000时，output=0.99999999，迭代次数为250000时，output＝1.
for iteration in xrange(1000000):
    output = (exp(dot(training_set_inputs,synaptic_weights)) - exp(-(dot(training_set_inputs,synaptic_weights)))) / (exp(dot(training_set_inputs,synaptic_weights)) + exp(-(dot(training_set_inputs,synaptic_weights))))
    synaptic_weights += dot(training_set_inputs.T,(training_set_outputs-output)*(1 + output) * (1-output))
print (exp(dot(array([1,0,0]),synaptic_weights)) - exp(-(dot(array([1,0,0]),synaptic_weights)))) / (exp(dot(array([1,0,0]),synaptic_weights)) + exp(-(dot(array([1,0,0]),synaptic_weights))))
end=time.time()
print "Run time: "
print end-start
