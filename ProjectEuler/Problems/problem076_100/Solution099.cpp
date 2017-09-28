// https://projecteuler.net/problem=99
/*
Problem 99

Largest exponential

Comparing two numbers written in index form like 211 and 37 is not difficult,
as any calculator would confirm that 2^11 = 2048 < 3^7 = 2187.

However, confirming that 632382^518061 > 519432^525806 would be much more difficult,
as both numbers contain over three million digits.

Using base_exp.txt (right click and 'Save Link/Target As...'), a 22K text file
containing one thousand lines with a base/exponent pair on each line, determine
which line number has the greatest numerical value.

NOTE: The first two lines in the file represent the numbers in the example given above.

Solution:
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;

template<class T>
struct indexValuePair {
    int index;
    T value;
};

auto readValuesFromFile(const string& filename) {
    vector<indexValuePair<double>> result;
    ifstream infile{ filename };
    if (infile.good()) {
        char space;
        for (int i = 1, b, e; !infile.eof(); i++) {
            infile >> b >> space >> e;
            result.push_back({ i, log(b)*e });
        }
    }
    else {
        cout << "File not found. \n";
    }
    return result;
}

auto compute() {
    auto allValues = readValuesFromFile(".\\Data\\p099_base_exp.txt");

    nth_element(allValues.begin(), allValues.end() - 1, allValues.end(),
        [](auto& x, auto& y) { return x.value < y.value; });

    return allValues.back().index;
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