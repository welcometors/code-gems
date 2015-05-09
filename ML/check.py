#!/usr/bin/env python

import json
import math
import numpy
import sys


def check(expected, output):
    correct = {}

    expectedlines = expected.splitlines()
    outputlines = output.splitlines()

    if len(outputlines) != len(expectedlines):
        print >> sys.stderr, 'Incorrect number of rows'
        return 0.0

    num_correct = 0
    num_false_positive = 0
    num_false_negative = 0
    num_wrong = 0
    total = 0

    for l in expectedlines:
        data = l.split(' ')
        correct[(data[0], data[1])] = int(data[2])
        total += 1

    for l in outputlines:
        data = l.split(' ')
        key = (data[0], data[1])
        actual_val = int(data[2])
        expected_val = correct.get(key)

        if actual_val == expected_val:
            num_correct += 1
        elif actual_val == 0 and expected_val == 1:
            num_false_negative += 1
        elif actual_val == 1 and expected_val == 0:
            num_false_positive += 1
        else:
            num_wrong += 1

    print ('Total:' + str(total))
    score = 1.0 * num_correct / total

    print ('Num correct: {0} ({1:.2f})'.format(num_correct, 100.*num_correct/total))
    print ('Num false positive: {0} ({1:.2f})'.format(num_false_positive, 100.*num_false_positive/total))
    print ('Num false negative: {0} ({1:.2f})'.format(num_false_negative, 100.*num_false_negative/total))
    print ('Num wrong type: {0} ({1:.2f})'.format(num_wrong, 100.*num_wrong/total))

    return score


def main():
    check(open(sys.argv[1]).read(), open(sys.argv[2]).read())


if __name__ == '__main__':
    main()