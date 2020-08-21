import re
import sys
import time
import timeit
import random
import numpy
import scipy
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import AdaBoostClassifier
from sklearn.cluster import KMeans
#from imblearn.over_sampling import SMOTE

DEFAULT_DIR = "./hackerrank-predict-email-opens-dataset/"
FULL_TRAINING_SET = DEFAULT_DIR + "training_dataset.csv"
LOCAL_TRAINING_SET = DEFAULT_DIR + "training_dataset_local.csv"
LOCAL_TEST_SET = DEFAULT_DIR + "test_dataset_local.csv"
TEST_SET = DEFAULT_DIR + "test_dataset.csv"
OUTPUT_SET = DEFAULT_DIR + "prediction.csv"

def createLocalSets(trainingPercent):
    header = []
    data = []
    with open(FULL_TRAINING_SET, 'r') as file:
        for line in file:
            if len(header) == 0:
                header = line
            else:
                data.append(line)

    random.shuffle(data)
    cutoff = int(len(data)*max(1,min(99,trainingPercent))/100)
    
    with open(LOCAL_TRAINING_SET, 'w') as file:
        file.write(header)
        for i in range(cutoff):
            file.write(data[i])

    with open(LOCAL_TEST_SET, 'w') as file:
        file.write(header)
        for i in range(cutoff,len(data)):
            file.write(data[i])

def numberExtractor(str): 
    number = 0
    for c in str:
        v = ord(c)
        if 48 <= v and v <= 57:
            number = number*10 + v
    return [number]
def timeType(str): 
    if str == '':
        return [0]
    return [int(str)]
    timestamp = int(str)
    date = time.localtime(timestamp)
    return [timestamp, date.tm_year, date.tm_mon, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec, date.tm_wday, date.tm_yday, date.tm_isdst]
def timeZoneType(str): return [0] if str == '' else [int(str)/200]
def intType(str): return [0] if str == '' else [int(str)]
def boolType(str): return [1] if str.lower() =='true' else [0]
def stringHash(str): return [int(hash(str))]

def getHeaderWiseData(data, header, list):
    row = []
    for val in list:
        col, func = val
        row.extend(func(data[header[col]]))
    return row

def extractData(filename, xList, yCol, yFunc, numRows=-1):
    header = {}
    xData = []
    yData = []
    with open(filename, 'r') as file:
        for line in file:
            if len(header) == 0:
                idx = 0
                for col in line.strip().split(','):
                    header[col] = idx
                    idx += 1
            else:
                if numRows == 0:
                    break;
                data = line.strip().split(',')
                xData.append(getHeaderWiseData(data, header, xList))
                yData.extend(yFunc(data[header[yCol]]))
                numRows -= 1
    return xData, yData

def getPrecisionAndRecall(groundTruth, predictedOutput):
    tp, tn, fp, fn = 0, 0, 0, 0
    for i in range(len(groundTruth)):
        if groundTruth[i] == predictedOutput[i]:
            if groundTruth[i] == 1:
                tp += 1
            else:
                tn += 1
        else:
            if predictedOutput[i] == 1:
                fp += 1
            else:
                fn += 1
    print("Summary   = {{tp({}), tn({}), fp({}), fn({})}}".format(tp, tn, fp, fn))
    print("Accuracy  = {}%".format(100*(tp+tn)/len(groundTruth)))
    precision = 0 if tp == 0 else tp/(tp+fp)
    recall =  0 if tp == 0 else tp/(tp+fn)
    return precision, recall

def fitGradientBoostingClassifier(trainX, trainY, ratio):
    clf = GradientBoostingClassifier(n_estimators=500, learning_rate=.5, max_depth=1)
    #clf = GradientBoostingClassifier(n_estimators=int(sys.argv[3]), learning_rate=float(sys.argv[4]), max_depth=int(sys.argv[5]))
    sampleWeights = [ratio if v == 1 else 1 for v in trainY]
    clf = clf.fit(trainX, trainY, sampleWeights)
    #clf = clf.fit(trainX, trainY)
    #F-score   = 0.194
    return clf

def fitAdaBoostClassifier(trainX, trainY, ratio):
    rfc = RandomForestClassifier(n_estimators=10, class_weight={1:ratio, 0:1}, criterion='gini', min_samples_split=7, min_samples_leaf=4, max_features='auto')
    clf = AdaBoostClassifier(base_estimator=rfc, n_estimators=5, learning_rate=1.0, algorithm='SAMME.R', random_state=True)
    #clf = RandomForestClassifier(n_estimators=100, class_weight={1:ratio, 0:1})
    clf = clf.fit(trainX, trainY)
    #F-score   = 0.15114936496273748
    return clf

def fitSVMClassifier(trainX, trainY, ratio):
    clf = SVC(kernel='linear', C=1.0, class_weight={1:ratio,0:1}, verbose=2)
    clf = clf.fit(trainX, trainY)
    return clf

def getClassRatio(dataY):
    sumY = sum(dataY)
    ratio = int(round((len(dataY)-sumY)/sumY))
    print('Class ratio 1:{}'.format(ratio))
    return ratio

def getFeatureRows():
    featureRows = [
        ("user_id", stringHash),
        ("mail_id", stringHash),
        ("mail_category", numberExtractor),
        ("mail_type", numberExtractor),
        ("sent_time", timeType),
        ("last_online", timeType),
        ("hacker_created_at", timeType),
        ("hacker_timezone", timeZoneType),
        ("contest_login_count", intType),
        ("contest_login_count_1_days", intType),
        ("contest_login_count_30_days", intType),
        ("contest_login_count_365_days", intType),
        ("contest_login_count_7_days", intType),
        ("contest_participation_count", intType),
        ("contest_participation_count_1_days", intType),
        ("contest_participation_count_30_days", intType),
        ("contest_participation_count_365_days", intType),
        ("contest_participation_count_7_days", intType),
        ("forum_comments_count", intType),
        ("forum_count", intType),
        ("forum_expert_count", intType),
        ("forum_questions_count", intType),
        ("hacker_confirmation", boolType),
        ("ipn_count", intType),
        ("ipn_count_1_days", intType),
        ("ipn_count_30_days", intType),
        ("ipn_count_365_days", intType),
        ("ipn_count_7_days", intType),
        ("ipn_read", intType),
        ("ipn_read_1_days", intType),
        ("ipn_read_30_days", intType),
        ("ipn_read_365_days", intType),
        ("ipn_read_7_days", intType),
        ("submissions_count", intType),
        ("submissions_count_1_days", intType),
        ("submissions_count_30_days", intType),
        ("submissions_count_365_days", intType),
        ("submissions_count_7_days", intType),
        ("submissions_count_contest", intType),
        ("submissions_count_contest_1_days", intType),
        ("submissions_count_contest_30_days", intType),
        ("submissions_count_contest_365_days", intType),
        ("submissions_count_contest_7_days", intType),
        ("submissions_count_master", intType),
        ("submissions_count_master_1_days", intType),
        ("submissions_count_master_30_days", intType),
        ("submissions_count_master_365_days", intType),
        ("submissions_count_master_7_days", intType)
    ]
    return featureRows

def upsampleData(dataX, dataY, ratio=1):
    allIndices = []
    positiveIndices = []
    for i in range(len(dataY)):
        if dataY[i] == 1:
            positiveIndices.append(i)
        allIndices.append(i)
    numPositive = int((len(allIndices)-len(positiveIndices))*ratio)
    while numPositive > 0:
        allIndices.append(positiveIndices[int(random.random() * len(positiveIndices))])
        numPositive -= 1
    random.shuffle(allIndices)
    random.shuffle(allIndices)
    newX = []
    newY = []
    for i in allIndices:
        newX.append(dataX[i])
        newY.append(dataY[i])
    return newX, newY

def downsampleData(dataX, dataY, ratio=1):
    negativeIndices = []
    positiveIndices = []
    for i in range(len(dataY)):
        if dataY[i] == 1:
            positiveIndices.append(i)
        else:
            negativeIndices.append(i)
    numNegative = int(len(positiveIndices)*ratio)
    while numNegative > 0:
        positiveIndices.append(negativeIndices[int(random.random() * len(negativeIndices))])
        numNegative -= 1
    random.shuffle(positiveIndices)
    random.shuffle(positiveIndices)
    newX = []
    newY = []
    for i in positiveIndices:
        newX.append(dataX[i])
        newY.append(dataY[i])
    return newX, newY

def downsampleDataKmeans(dataX, dataY):
    negativeData = []
    positiveData = []
    for i in range(len(dataY)):
        if dataY[i] == 1:
            positiveData.append(dataX[i])
        else:
            negativeData.append(dataX[i])
    cls = KMeans(n_clusters=len(positiveIndices), random_state=random_state)
    cls.fit_predict(negativeData)
    numNegative = int(*ratio)
    while numNegative > 0:
        positiveIndices.append(negativeIndices[int(random.random() * len(negativeIndices))])
        numNegative -= 1
    random.shuffle(positiveIndices)
    random.shuffle(positiveIndices)
    newX = []
    newY = []
    for i in positiveIndices:
        newX.append(dataX[i])
        newY.append(dataY[i])
    return newX, newY

def train(trainingFile, featureRows, numRows):
    print('Reading training file ...')
    trainX, trainY = extractData(trainingFile, featureRows, "clicked", boolType, numRows)
    print('Done! {} data rows.'.format(len(trainX)))
    ratio = getClassRatio(trainY)
    
    print('Resampling ...')
    trainX, trainY = downsampleData(trainX, trainY)
    print('Done! {} data rows.'.format(len(trainX)))
    ratio = getClassRatio(trainY)

    print('Training with {} features ...'.format(len(trainX[0])))
    start = timeit.default_timer()
    clf = fitGradientBoostingClassifier(trainX, trainY, ratio)
    stop = timeit.default_timer()
    print('Training time = {} sec.'.format(stop-start))
    return clf

def test(testFile, featureRows, clf):
    print('Reading test file ...')
    testX, testY = extractData(testFile, featureRows, "clicked", boolType)
    print('Done! {} data rows.'.format(len(testX)))
    getClassRatio(testY)

    print('Testing ...')
    predictedY = clf.predict(testX)
    precision, recall = getPrecisionAndRecall(testY, predictedY)
    accuracy = 0 if precision+recall == 0 else 2*precision*recall/(precision+recall)
    print("Precision = {}".format(precision))
    print("Recall    = {}".format(recall))
    print("F-score   = {}".format(accuracy))

def run(runFile, featureRows, clf, outputFile):
    print('Reading test file ...')
    testX, testY = extractData(runFile, featureRows, "user_id", stringHash)
    print('Done! {} data rows.'.format(len(testX)))
    getClassRatio(testY)

    print('Evaluating ...')
    predictedY = clf.predict(testX)
    with open(outputFile, 'w') as file:
        for val in predictedY:
            file.write("{}\n".format(val))

if __name__ == '__main__':
    if len(sys.argv) > 1:
        if sys.argv[1] == 'init':
            createLocalSets(70 if len(sys.argv) < 3 else float(sys.argv[2]))
        elif sys.argv[1] == 'test':
            featureRows = getFeatureRows()
            clf = train(LOCAL_TRAINING_SET, featureRows, -1 if len(sys.argv) < 3 else int(sys.argv[2]))
            test(LOCAL_TEST_SET, featureRows, clf)
        elif sys.argv[1] == 'run':
            featureRows = getFeatureRows()
            clf = train(FULL_TRAINING_SET, featureRows, -1)
            run(TEST_SET, featureRows, clf, OUTPUT_SET)