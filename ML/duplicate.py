import json
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.ensemble import GradientBoostingClassifier

def Union(s1, s2):
    a = set(s1)
    b = set(s2)
    u = a.union(b)
    return list(u)

def Intersection(s1, s2):
    a = set(s1)
    b = set(s2)
    i = a.intersection(b)
    return list(i)

def AreContextEquals( ctxA, ctxB):
    if ctxA == None or ctxB == None:
        return 0
    if ctxA['name'] == ctxB['name']:
        return 1
    else:
        return 0

def GetTopicSim(T1, T2):
    nT1 = {}
    nT2 = {}
    if T1==None or T2 == None or len(T1) == 0 or len(T2) == 0:
        return 0
    for t in T1:
        nT1[t['name']] = t['followers']
    for t in T2:
        nT2[t['name']] = t['followers']
    u = Union(nT1.keys(), nT2.keys())
    i = Intersection(nT1.keys(), nT2.keys())
    num = 0
    den = 0
    for s in i:
        num = num + nT1[s]
    for s in u:
        if s in nT1:
            den = den + nT1[s]
        else:
            den = den + nT2[s]
    if den == 0:
        return 0
    return float(num)/float(den)

def GetFeatures(TrainingData, QuestionIDs, WordTfs, Views, ContextTopics, Topics, FollowCounts, Ages):
    idx1 = QuestionIDs[TrainingData[0]]
    idx2 = QuestionIDs[TrainingData[1]]
    qTextSim = WordTfs[idx1].dot(WordTfs[idx2].transpose()).toarray().item()
    ContextSim = AreContextEquals(ContextTopics[idx1], ContextTopics[idx2])
    TopicSim = GetTopicSim(Topics[idx1], Topics[idx2])
    return [Views[idx1], Views[idx2], qTextSim, ContextSim, TopicSim, FollowCounts[idx1], FollowCounts[idx2], Ages[idx1], Ages[idx2]]

def TrainData( TrainingPairs, QuestionIDs, WordTfs, Views, ContextTopics, Topics, FollowCounts, Ages):
    XTrain = []
    YTrain = []
    for tp in TrainingPairs:
        XTrain.append(GetFeatures(tp, QuestionIDs, WordTfs, Views, ContextTopics, Topics, FollowCounts, Ages))
        YTrain.append(int(tp[2]))
    Data = [YTrain, XTrain]
    return Data

RawData = []
Questions = int(input())

while Questions:
    Questions = Questions - 1
    RawData.append(json.loads(input()))


TrainingPairs = []
Duplicates = int(input())

while Duplicates:
    Duplicates = Duplicates - 1
    tok = input().split(' ')
    TrainingPairs.append((tok[0], tok[1], tok[2].replace('\n','')))

TestPair = []
Queries = int(input())

while Queries:
    Queries = Queries - 1
    tok = input().split(' ')
    TestPair.append((tok[0], tok[1]))

#print ('Training Data')

CVectorizer = CountVectorizer(min_df=1)
TfIdfVectorizer = TfidfTransformer()

KeyCount = 0
Views = []
FollowCounts = []
Ages = []
QuestionIDs = {}
QuestionTexts = []
ContextTopics = []
Topics = []

for data in RawData:
    Views.append(int(data['view_count']))
    followCount = '0' if data['follow_count'] is None else data['follow_count']
    FollowCounts.append(int(followCount))
    age = '0' if data['age'] is None else data['age']
    Ages.append(int(age))
    QuestionIDs[data['question_key']] = KeyCount
    QuestionTexts.append(data['question_text'])
    ContextTopics.append(data['context_topic'])
    Topics.append(data['topics'])
    KeyCount = KeyCount + 1

Words = CVectorizer.fit_transform(QuestionTexts)
WordsTfs = TfIdfVectorizer.fit_transform(Words)
del Words
    
TrainedData = TrainData(TrainingPairs, QuestionIDs, WordsTfs, Views, ContextTopics, Topics, FollowCounts, Ages)

clf = GradientBoostingClassifier(n_estimators=200, learning_rate=.7, max_depth=1)
clf = clf.fit(TrainedData[1], TrainedData[0])
#print ('Trained')

for tp in TestPair:
    x = GetFeatures(tp, QuestionIDs, WordsTfs, Views, ContextTopics, Topics, FollowCounts, Ages)
    y = clf.predict(x)
    print (tp[0] + ' ' + tp[1] + ' ' + str(y.item()))
