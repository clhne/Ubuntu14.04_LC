#-*-coding:utf-8-*-
import pandas as pd
import numpy as np
import random
import math
import time

start = time.time()
#1.按类别划分样本
#separated = {0: [[att1, att2, ... att8, 0], ...],
#             1: [[att1, att2, ... att8, 1], [att1, att2, ... att8, 1], ...]}
def separate_by_class(dataset):
    separated = {}
    for i in range(len(dataset)):
        vector = dataset[i]
        if (vector[-1] not in separated):
            separated[vector[-1]] = []
        separated[vector[-1]].append(vector)
    return separated


#2.提取属性特征. 对一个类的所有样本,计算每个属性的均值和方差,
#summaries = [(att1_mean,att1_stdev), (att2_mean,att2_stdev), .., (att8_mean,att8_stdev)]
def mean(numbers):
    return sum(numbers)/float(len(numbers))
def stdev(numbers):
    avg = mean(numbers)
    variance = sum([pow(x-avg,2) for x in numbers])/float(len(numbers)-1)
    return math.sqrt(variance)
def summarize(dataset):
    summaries = [(mean(attribute), stdev(attribute)) for attribute in  zip(*dataset)]
    del summaries[-1]
    return summaries


#3.按类别提取属性特征
#summaries = {0:[(att1_mean,att1_stdev), (att2_mean,att2_stdev), .., (att8_mean,att8_stdev)],
#             1:[(att1_mean,att1_stdev), (att2_mean,att2_stdev), .., (att8_mean,att8_stdev)]}
def summarize_by_class(dataset):
    separated = separate_by_class(dataset)
    summaries = {}
    keyList = list(separated.keys())
    for classValue in keyList:
        summaries[classValue] = summarize(separated[classValue])
    return summaries


#计算高斯概率密度函数. 计算样本的某一属性x的概率,归属于某个类的似然
def calculate_probability(x, mean, stdev):
    exponent = math.exp(-(math.pow(x-mean,2)/(2*math.pow(stdev,2))))
    return (1 / (math.sqrt(2*math.pi) * stdev)) * exponent


#4.对一个样本,计算它属于每个类的概率
def calculate_class_probabilities(summaries, inputVector):
    probabilities = {}
    keyList = list(summaries.keys())
    for classValue in keyList:
        probabilities[classValue] = 1
        for i in range(len(summaries[classValue])):  #属性个数
            mean, stdev = summaries[classValue][i]   #训练得到的第i个属性的提取特征
            x = inputVector[i]                       #测试样本的第i个属性x
            probabilities[classValue] *= calculate_probability(x, mean, stdev)
    return probabilities


#5.单个数据样本的预测. 找到最大的概率值,返回关联的类
def predict(summaries, inputVector):
    probabilities = calculate_class_probabilities(summaries, inputVector)
    bestLabel, bestProb = None, -1
    keyList = list(probabilities.keys())
    for classValue in keyList:
        if bestLabel is None or probabilities[classValue] > bestProb:
            bestProb = probabilities[classValue]
            bestLabel = classValue
    return bestLabel


#多个数据样本的预测
def get_predictions(summaries, testSet):
    predictions = []
    for i in range(len(testSet)):
        result = predict(summaries, testSet[i])
        predictions.append(result)
    return predictions

#6.计算精度
def get_accuracy(testSet, predictions):
    correct = 0
    for x in range(len(testSet)):
        if testSet[x][-1] == predictions[x]:
            correct += 1
    return (correct/float(len(testSet)))*100.0


def main():
    #读取数据
    filename = 'pima-indians-diabetes.csv'
    dataset = pd.read_csv(filename, header=None)
    dataset = np.array(dataset)

    #随机划分数据:67%训练和33%测试
    trainSize = int(len(dataset)*2/3) #(512,9)(256,9)
    randomIdx = [i for i in range(len(dataset))]
    random.shuffle(randomIdx)
    trainSet = []
    testSet = []
    trainSet.extend(dataset[idx,:] for idx in randomIdx[:trainSize])
    testSet.extend(dataset[idx,:] for idx in randomIdx[trainSize:])

    #计算模型
    summaries = summarize_by_class(trainSet)

    #用测试数据集测试模型
    predictions = get_predictions(summaries, testSet)
    accuracy = get_accuracy(testSet, predictions)
    print(('Accuracy:{0}%').format(accuracy))
    end = time.time()
    print'Run time: '
    print end - start


if __name__ == '__main__':
    main()

