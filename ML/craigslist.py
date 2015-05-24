import io
import sys
import json
import re
import numpy
import scipy
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.preprocessing import LabelEncoder
from sklearn.svm import LinearSVC
#from sklearn.linear_model import SGDClassifier
#from sklearn.linear_model import RidgeClassifier
#from sklearn.linear_model import Perceptron
#from sklearn.linear_model import PassiveAggressiveClassifier

def GetFeatures(city, section, trainingWords):
    return [city, section, trainingWords]

def GetOrCreateAndGet( dict, key, value):
    if key not in dict:
        dict[key] = value
        return value
    return dict[key]

class craigslist(object):
    encoding = 'utf-8'
    jasonPattern = re.compile("[^a-zA-Z0-9:{}\\.\\-$,\"\\\\ ]")
    headingPattern = re.compile("[^a-zA-Z0-9\\. ]")
    removeExtraSpacePattern = re.compile("\s+")
    tainingSize = 0
    le = LabelEncoder()
    categorySet = ['activities','appliances','artists','automotive','cell-phones','childcare','general', 'household-services','housing','photography','real-estate','shared','temporary','therapeutic', 'video-games','wanted-housing']
    le.fit(categorySet)
    categories = []
    headings = []
    
    with open("craigslistTraining.json", 'r', encoding=encoding) as f:
        tainingSize = int(f.readline());
        for i in range(tainingSize):
            line = f.readline().strip()
            row = json.loads(jasonPattern.sub(' ', line))
            categories.append(row['category'])
            text = row['city'] + ' ' + row['section'] + ' ' + row['heading']
            headings.append(removeExtraSpacePattern.sub(' ', headingPattern.sub(' ', text)))

    categories = le.transform(categories)
    inputStream = io.TextIOWrapper(sys.stdin.buffer, encoding=encoding)
    sampleSize = int(inputStream.readline())
    for i in range(sampleSize):
        line = inputStream.readline()
        row = json.loads(jasonPattern.sub(' ', line))
        text = row['city'] + ' ' + row['section'] + ' ' + row['heading']
        headings.append(removeExtraSpacePattern.sub(' ', headingPattern.sub(' ', text)))
    
    TfIdfVectorizer = TfidfVectorizer(ngram_range=(1,2))
    wordsTfs = TfIdfVectorizer.fit_transform(headings)
    trainingWords = wordsTfs[:tainingSize]
    sampleWords = wordsTfs[tainingSize:]

    clf = LinearSVC(loss='hinge', class_weight='auto')
    clf = clf.fit(trainingWords, categories)

    match = le.inverse_transform(clf.predict(sampleWords))
    for c in match:
        print(c)

__main__ = craigslist
