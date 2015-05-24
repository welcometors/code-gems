import math
import numpy as np
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.preprocessing import MinMaxScaler
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import AdaBoostClassifier

n, m = map(int,input().split(' '))
data = [];
groups = [];
while n > 0:
    cols = input().split(' ')
    dims = [0.0] * m
    for pair in cols[2:]:
        vals = pair.split(':')
        dims[int(vals[0]) - 1] = float(vals[1])
    dims.append(math.log(dims[0]))
    data.append(dims)
    groups.append(int(cols[1]))
    n-=1

X = np.array(data)
#print("done! training...")
#clf = GradientBoostingClassifier(n_estimators=200, learning_rate=.7, max_depth=2)
rfc = RandomForestClassifier(n_estimators=10, criterion='gini',  min_samples_split=7, min_samples_leaf=4, max_features='auto')
clf = AdaBoostClassifier(base_estimator=rfc, n_estimators=5, learning_rate=1.0, algorithm='SAMME.R', random_state=True)
clf.fit(X, groups)

t = int(input())
keys = []
test = []
while t > 0:
    cols = input().split(' ')
    dims = [0.0] * m
    for pair in cols[1:]:
        vals = pair.split(':')
        dims[int(vals[0]) - 1] = float(vals[1])
    dims.append(math.log(dims[0]))
    keys.append(cols[0])
    test.append(dims)
    t-=1

Z = np.array(test)
#nrml = MinMaxScaler()
#nrml.fit(X)
#X = nrml.transform(X)
#Z = nrml.transform(Z)

match = clf.predict(Z)
for i in range(0, len(keys)):
    print("{0} {1}".format( keys[i], "+1" if match[i] > 0 else "-1"))
