from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.preprocessing import MultiLabelBinarizer
from sklearn.multiclass import OneVsRestClassifier
from sklearn.svm import LinearSVC
from sklearn.svm import SVC
import numpy

CVectorizer = CountVectorizer(min_df=1)
TfIdfVectorizer = TfidfTransformer()
T, E = map(int, input().split(' '))
RawData = []
Labels = []
for i in range(T) :
    labels = map(int, input().split(' '))
    RawData.append(input())
    Labels.append(labels)

Queries = []
for i in range(E) :
    Queries.append(input())

RawData.extend(Queries)
X = CVectorizer.fit_transform(RawData)
Xtf = TfIdfVectorizer.fit_transform(X)
del X

MLB = MultiLabelBinarizer()
Yt = MLB.fit_transform(Labels)
XtfTrain = Xtf[0:T]
XtfTest = Xtf[T:]
Clf = OneVsRestClassifier(LinearSVC(loss='l1', class_weight={1:100,0:1})).fit(XtfTrain, Yt)
Classes = list(MLB.classes_)

for xTest in XtfTest:
    y = Clf.decision_function(xTest)
    y1 = list(y[0])
    c1 = Classes
    lbls = [x for (y,x) in sorted(zip(y1,c1))][-10:]
    list.reverse(lbls)
    print (' '.join([str(i) for i in lbls]))