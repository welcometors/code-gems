import numpy
import scipy
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.feature_extraction.text import TfidfVectorizer

n = 0
docs = []
label = []
with open("DocumentClassification.txt", 'r') as f:
    n = int(f.readline());
    for i in range(n):
        l = f.readline().strip().split(' ')
        docs.append(' '.join(l[1:]))
        label.append(int(l[0]))

t = int(input())
for i in range(t):
    docs.append(input().strip())

TfIdfVectorizer = TfidfVectorizer(min_df=1)
WordsTfs = scipy.sparse(TfIdfVectorizer.fit_transform(docs))
TrainingWords = WordsTfs[:n]
PredictWords = WordsTfs[n:]

clf = c(n_estimators=200, learning_rate=.7, max_depth=1)

clf.fit(TrainingWords, label)

for w in PredictWords:
    print(clf.predict(w))
# northern telecom proposes two for one stock split