// https://projecteuler.net/problem=42
/*
Coded triangle numbers

The nth term of the sequence of triangle numbers is given by, tn = 1/2n(n+1); 
so the first ten triangle numbers are:

1, 3, 6, 10, 15, 21, 28, 36, 45, 55, ...

By converting each letter in a word to a number corresponding to its alphabetical position 
and adding these values we form a word value. For example, the word value for 
SKY is 19 + 11 + 25 = 55 = t10. 
If the word value is a triangle number then we shall call the word a triangle word.

Using words.txt (right click and 'Save Link/Target As...'), a 16K text file containing nearly 
two-thousand common English words, how many are triangle words?
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <chrono>
using namespace std;

int term(int n) {
	n = 1 + 8 * n;
	int t = sqrt(n);
	if (t*t == n && t % 2 == 1)
		return (t - 1) / 2;
	return -1;
}

unsigned compute() {
	unsigned count = 0;
	ifstream file(".\\Data\\p042_words.txt");
	if (file.good()) {
		int wordValue = 0;
		string text;
		file >> text;
		text += ',';
		for (char c : text) {
			if (c == ',') {
				if (term(wordValue) != -1)
					count++;
				wordValue = 0;
			}
			else if (c != '"')
				wordValue += c - 'A' + 1;
		}
	}
	else {
		cout << "File not found. \n";
	}

	return count;
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
