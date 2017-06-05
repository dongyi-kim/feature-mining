import trees
import csv


featureLabels = ['Feature Mean #%d' % (i + 1) for i in range(0, 64)]
featureDividers = []
dividers = open('mean.csv','r')
featureDividers = list(map(float,dividers.readline().split(', ')))
dividers.close()

def dataToFeatureVector(line):
    global featureDividers
    splited = line.strip().split(', ')
    features = []
    for i in range(1, len(splited)):
        value = float(splited[i])
        divider = featureDividers[i - 1]
        if value <= divider:
            features.append('less')
        else:
            features.append('greater')

    features.append(int(splited[0]))
    return features

def loadDataSet(fileName):
    dataFile = open(fileName, 'r')
    dataSet = []
    for line in dataFile.readlines():
        features = dataToFeatureVector(line)
        dataSet.append(features)
    dataFile.close()
    return dataSet


decisionTree = trees.createTree(loadDataSet('cifar10.train.vector.csv'), featureLabels, 10)
print('[Done] DecisionTree Generation')

numberOfTest = 0
numberOfSuccess = 0
numberOfFail = 0

featureLabels = ['Feature Mean #%d' % (i + 1) for i in range(0, 64)]
for features in loadDataSet('cifar10.test.vector.csv'):
    answer = features[-1]
    predicted = trees.predictClass(decisionTree, features, featureLabels)
    numberOfTest += 1
    if predicted == answer:
        numberOfSuccess += 1
    else:
        numberOfFail += 1

print('[Done] Test')
print('[Result] %.4f' % (float(numberOfSuccess/numberOfTest)))

