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

auto getPairsFromFile(const string& filename) {
	vector<pair<double, int>> basePairs;
	ifstream infile{ filename };
	if (infile.good()) {
		char c;
		for (int i = 1, b, e; !infile.eof(); i++) {
			infile >> b >> c >> e;
			basePairs.push_back({ log(b)*e, i });
		}
	}
	else {
		cout << "File not found. \n";
	}
	return basePairs;
}

auto compute() {
	auto pairs = getPairsFromFile(".\\Data\\p099_base_exp.txt");
	
	nth_element(pairs.begin(), pairs.end() - 1, pairs.end(),
		[](auto& x, auto& y) {
			return x.first < y.first;
		});

	return pairs.back().second;
}

#ifdef _MSC_VER
	template <class T>
	inline void DoNotOptimize(const T &value) {
		__asm { lea ebx, value }
	}
#else
	template <class T>
	__attribute__((always_inline)) inline void DoNotOptimize(const T &value) {
		asm volatile("" : "+m"(const_cast<T &>(value)));
	}
#endif

int main() {
	using namespace std;
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	DoNotOptimize(result);
	cout << "Done in "
		<< duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6
		<< " miliseconds." << endl;
	cout << result << endl;
}