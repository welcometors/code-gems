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

def GetFeatures(city, section, heading):
    return city + ' ' + section + ' ' + heading

class craigslist(object):
    encoding = 'utf-8'
    tainingSize = 0
    le = LabelEncoder()
    categorySet = ['activities','appliances','artists','automotive','cell-phones','childcare','general', 'household-services','housing','photography','real-estate','shared','temporary','therapeutic', 'video-games','wanted-housing']
    le.fit(categorySet)
    categories = []
    headings = []
    
    with open("training.json", 'r', encoding=encoding) as f:
        tainingSize = int(f.readline());
        for i in range(tainingSize):
            row = json.loads(f.readline().strip())
            categories.append(row['category'])
            headings.append( GetFeatures( row['city'], row['section'], row['heading']))

    categories = le.transform(categories)
    inputStream = io.TextIOWrapper(sys.stdin.buffer, encoding=encoding)
    sampleSize = int(inputStream.readline())
    for i in range(sampleSize):
        row = json.loads(inputStream.readline().strip())
        headings.append( GetFeatures( row['city'], row['section'], row['heading']))
    
    TfIdfVectorizer = TfidfVectorizer(ngram_range=(1,2))
    wordsTfs = TfIdfVectorizer.fit_transform(headings)
    trainingWords = wordsTfs[:tainingSize]
    sampleWords = wordsTfs[tainingSize:]

    clf = LinearSVC()
    clf = clf.fit(trainingWords, categories)

    match = le.inverse_transform(clf.predict(sampleWords))
    for c in match:
        print(c)

__main__ = craigslist
