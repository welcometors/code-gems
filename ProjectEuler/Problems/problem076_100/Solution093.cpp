// https://projecteuler.net/problem=93
/*
Problem 93

By using each of the digits from the set, {1, 2, 3, 4}, exactly once, and making use of the 
four arithmetic operations (+, -, *, /) and brackets/parentheses, it is possible to form 
different positive integer targets.

For example,

8 = (4 * (1 + 3)) / 2
14 = 4 * (3 + 1 / 2)
19 = 4 * (2 + 3) - 1
36 = 3 * 4 * (2 + 1)

Note that concatenations of the digits, like 12 + 34, are not allowed.

Using the set, {1, 2, 3, 4}, it is possible to obtain thirty-one different target numbers of 
which 36 is the maximum, and each of the numbers 1 to 28 can be obtained before encountering 
the first non-expressible number.

Find the set of four distinct digits, a < b < c < d, for which the longest set of consecutive 
positive integers, 1 to n, can be obtained, giving your answer as a string: abcd.

Solution:
Using RPN for calculating expression
Precalculating all the possible positions of operator and operands and then putting numbers.
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <numeric>
#include <chrono>
using namespace std;

const string operators = "+-*/";
const float epsilon = 1e-6;

void fillOperators(vector<string>& ary, string& s, int n) {
    if (!n--) {
        ary.push_back(s);
        return;
    }
    for (char o : operators) {
        s[n] = o;
        fillOperators(ary, s, n);
    }
}

auto getOperators(int n) {
    vector<string> operatorCombinations;
    operatorCombinations.reserve(1 << (2 * n));
    string tmp(n, ' ');
    fillOperators(operatorCombinations, tmp, n);
    return operatorCombinations;
}

void fillPositions(vector<string>& ary, const string& s, int l, int r, int o) {
    auto e = s;
    if (!r) {
        while (o--)
            e += 'o';
        ary.emplace_back(e);
        return;
    }
    e.reserve(e.size() + l + 1);
    for (int i = 0; i <= l; i++) {
        e += 'd';
        fillPositions(ary, e, l + 1 - i, r - 1, o - i);
        e.back() = 'o';
    }
}

auto getPositions(int n) {
    vector<string> positionCombinations;
    fillPositions(positionCombinations, "", 1, n - 1, n);
    return positionCombinations;
}

int evaluate(const vector<int>& ary, const string& opr, const string& comb) {
    vector<float> stack(ary.size());
    stack[0] = ary[0];
    stack[1] = ary[1];

    int top = 1, oi = 0, ai = 2;
    for (auto& x : comb) {
        if (x == 'o') {
            if (opr[oi] == '+') {
                stack[top - 1] += stack[top];
            }
            else if (opr[oi] == '-') {
                stack[top - 1] -= stack[top];
            }
            else if (opr[oi] == '*') {
                stack[top - 1] *= stack[top];
            }
            else if (opr[oi] == '/') {
                if (stack[top] == 0)
                    return -1;
                stack[top - 1] /= stack[top];
            }
            oi++;
            top--;
        }
        else {
            stack[++top] = ary[ai++];
        }
    }
    if (top || stack.front() - int(stack.front()) > epsilon)
        return -1;
    return stack.front();
}

int getChainCount(vector<int>& ary, const vector<string>& oprs, const vector<string>& positions) {
    set<int> outcomes;
    do {
        for (auto& pos : positions)
            for (auto& opr : oprs) {
                int r = evaluate(ary, opr, pos);
                if (r > 0)
                    outcomes.insert(r);
            }
    } while (next_permutation(ary.begin(), ary.end()));

    int i = 0;
    for (auto x : outcomes)
        if (i + 1 == x)
            i++;
        else
            break;

    return i;
}

auto compute() {
    vector<int> combination = { 0,0,0,0,0,0,1,1,1,1 };
    int maxChain = 0, maxChainNumber = 0;
    do {
        vector<int> number;
        for (int digit = 0; digit < 10; digit++)
            if (combination[digit])
                number.push_back(digit);
        int chainCount = getChainCount(number, getOperators(3), getPositions(3));
        if (maxChain < chainCount) {
            maxChain = chainCount;
            maxChainNumber = accumulate(number.begin(), number.end(), 0, 
                [](auto s, auto x) {return s * 10 + x; });
        }
    } while (next_permutation(combination.begin(), combination.end()));
    return maxChainNumber;
}

template<typename Function, class ... Types>
decltype(auto) timeit(Function f, Types ... args) {
    using namespace chrono;
    auto start = high_resolution_clock::now();
    auto result = f(args...);
    double duration = duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6;
    return std::make_pair(result, duration);
}

int main() {
    using namespace std;
    auto[result, time] = timeit(compute);
    cout << result << " Calculated in " << time << " miliseconds." << '\n';
    return 0;
}