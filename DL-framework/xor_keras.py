#!/usr/bin/python2.7
#-*-coding:utf-8-*-
import numpy as np
import time
start = time.clock()
from keras.models import Sequential
from keras.layers.core import Activation, Dense
from keras.optimizers import SGD

X = np.array([[0,0],[0,1],[1,0],[1,1]], "float32")
y = np.array([[0],[1],[1],[0]], "float32")

model = Sequential()
model.add(Dense(2, input_dim=2, activation='sigmoid'))
model.add(Dense(1, activation='sigmoid'))

sgd = SGD(lr=0.1, decay=1e-6, momentum=0.9, nesterov=True)
model.compile(loss='mean_squared_error', optimizer=sgd)

history = model.fit(X, y, epochs=10000, batch_size=4, verbose=0)

print model.predict(X)
end = time.clock()
print 'Run time: '
print end - start
