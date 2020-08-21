// https://projecteuler.net/problem=89
/*
Problem 89

Roman numerals

For a number written in Roman numerals to be considered valid there are basic rules 
which must be followed. Even though the rules allow some numbers to be expressed 
in more than one way there is always a "best" way of writing a particular number.

For example, it would appear that there are at least six ways of writing the number 
sixteen:

IIIIIIIIIIIIIIII
VIIIIIIIIIII
VVIIIIII
XIIIIII
VVVI
XVI

However, according to the rules only XIIIIII and XVI are valid, and the last example 
is considered to be the most efficient, as it uses the least number of numerals.

The 11K text file, roman.txt (right click and 'Save Link/Target As...'), contains 
one thousand numbers written in valid, but not necessarily minimal, Roman numerals; 
see About... Roman Numerals for the definitive rules for this problem.

Find the number of characters saved by writing each of these in their minimal form.

Note: You can assume that all the Roman numerals in the file contain no more than 
four consecutive identical units.

Solution:
*/

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
using namespace std;

const int romanNumerals[26] = { 0,0,100,500,0,0,0,0,1,0,0,50,1000,0,0,0,0,0,0,0,0,5,0,10,0,0 };

int fromRoman(const string& s) {
	int last = 1000;
	int sum = 0;
	for (auto& c : s) {
		int v = romanNumerals[c - 'A'];
		if (v > last)
			sum -= 2 * last;
		sum += v;
		last = v;
	}
	return sum;
}

string toRoman(int val) {
	string num;
	for (; val >= 1000; val -= 1000)
		num += 'M';

	if (val >= 900) {
		num += "CM";
		val -= 900;
	}
	else if (val >= 500) {
		num += 'D';
		val -= 500;
	}
	else if (val >= 400) {
		num += "CD";
		val -= 400;
	}

	for (; val >= 100; val -= 100)
		num += 'C';

	if (val >= 90) {
		num += "XC";
		val -= 90;
	}
	else if (val >= 50) {
		num += 'L';
		val -= 50;
	}
	else if (val >= 40) {
		num += "XL";
		val -= 40;
	}

	for (; val >= 10; val -= 10)
		num += 'X';

	if (val >= 9) {
		num += "IX";
		val -= 9;
	}
	else if (val >= 5) {
		num += 'V';
		val -= 5;
	}
	else if (val >= 4) {
		num += "IV";
		val -= 4;
	}

	for (; val; val--)
		num += 'I';

	return num;
}

auto compute() {
	int saving = 0;

	std::ifstream file(".\\Data\\p089_roman.txt");
	if (file.good()) {
		while (!file.eof()) {
			string roman;
			file >> roman;
			string optimal = toRoman(fromRoman(roman));
			saving += roman.length() - optimal.length();
		}
	}
	else {
		cout << "File not found. \n";
	}

	return saving;
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