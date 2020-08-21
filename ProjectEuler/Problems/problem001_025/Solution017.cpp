// https://projecteuler.net/problem=17
/*
If the numbers 1 to 5 are written out in words: one, two, three, four, five, then there are 3 + 3 + 5 + 4 + 4 = 19 letters used in total.

If all the numbers from 1 to 1000 (one thousand) inclusive were written out in words, how many letters would be used?


NOTE: Do not count spaces or hyphens. For example, 342 (three hundred and forty-two) contains 23 letters and 115 
(one hundred and fifteen) contains 20 letters. The use of "and" when writing out numbers is in compliance with British usage.

Solution:
*/

#include <iostream>
#include <string>
using namespace std;

const string wordConstants[] = {
	"Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine",
	"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen",
	"Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety", 
	" Hundred", "", " Thousand", " Million", " Billion", " Trillion"
};

string toWords(uint64_t n) {
	if (n <= 20)
		return wordConstants[n];
	
	string text;
	auto lim = 1000000000000ULL;
	for (size_t idx = 33; idx >= 29; idx--, lim /= 1000) {
		if (n >= lim) {
			auto v = n / lim;
			if (v >= 100) {
				text += " " + wordConstants[v / 100] + wordConstants[28];
				v %= 100;
				if (v)
					text += " and";
			}
			if (v >= 20) {
				text += " " + wordConstants[18 + (v / 10)];
				v %= 10;
			}
			if (v)
				text += " " + wordConstants[v];
			text += wordConstants[idx];
			n %= lim;
		}
	}
	return text.substr(1);
}

int main() {
	int sum = 0;

	for (int i = 1; i <= 1000; i++)
		for (auto c : toWords(i))
			if (c != ' ')
				sum++;

	cout << sum << '\n';
}