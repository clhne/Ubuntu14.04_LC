#-*-coding:utf-8-*-
import time
from keras.models import Sequential
from keras.layers import Dense
import numpy
start = time.time()
# fix random seed for reproducibility
numpy.random.seed(7)
#load pima indians dataset
dataset = numpy.loadtxt("pima-indians-diabetes.csv",delimiter=",")
#split into input(x) output(y) variables
x = dataset[:,0:8]
y = dataset[:,8]
#create model
model = Sequential()
model.add(Dense(12,input_dim=8,activation='relu'))
model.add(Dense(8,activation='relu'))
model.add(Dense(1,activation='sigmoid'))
#compile model
model.compile(loss='binary_crossentropy',optimizer='adam',metrics=['accuracy'])
#fit the model
model.fit(x,y,epochs=150,batch_size=10)
#evaluate the model
#scores = model.evaluate(x,y)
#print("\n%s: %.2f%%" %(model.metrics_names[1],scores[1]*100))
#calculate predictions
predictions = model.predict(x)
#round predictions
rounded = [round(x[0]) for x in predictions]
print(rounded)
end = time.time()
print 'Run time: '
print end - start
