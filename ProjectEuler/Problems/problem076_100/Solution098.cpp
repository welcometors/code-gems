// https://projecteuler.net/problem=98
/*
Problem 98

Anagramic squares

By replacing each of the letters in the word CARE with 1, 2, 9, and 6 respectively, 
we form a square number: 1296 = 362. What is remarkable is that, by using the same 
digital substitutions, the anagram, RACE, also forms a square number: 9216 = 962. 
We shall call CARE (and RACE) a square anagram word pair and specify further that 
leading zeroes are not permitted, neither may a different letter have the same digital 
value as another letter.

Using words.txt (right click and 'Save Link/Target As...'), a 16K text file containing 
nearly two-thousand common English words, find all the square anagram word pairs 
(a palindromic word is NOT considered to be an anagram of itself).

What is the largest square number formed by any member of such a pair?

NOTE: All anagrams formed must be contained in the given text file.

Solution:
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <chrono>
using namespace std;
using wordHash = unsigned long long;

#ifdef _MSC_VER
#include <intrin.h>
#include <cstdint>
uint32_t __inline __builtin_clz(uint32_t value) {
	unsigned long leading_zero = 0;

	if (_BitScanReverse(&leading_zero, value))
		return 31 - leading_zero;

	// Undefined, 32 seems better than 0
	return 32;
}
#endif

// undefined for 0
auto encodeEliasDelta32(uint32_t x) {
	uint32_t n = 31 - __builtin_clz(x);
	x = ((n + 1) << n) | (x - (1 << n));
	n = n + 1 + ((31 - __builtin_clz(n + 1)) << 1);
	return make_pair(x, n);
}

// supports 10 characters without collision
auto getEliasDeltaHash64(const string& str) {
	uint32_t freq[26] = {};
	for (auto& c : str)
		++freq[c - 'A'];

	wordHash h = 0;
	for (uint32_t i = 0; i < 26; ++i)
		if (freq[i]) {
			h <<= 5;
			h |= i;
			auto[x, l] = encodeEliasDelta32(freq[i]);
			h <<= l;
			h |= x;
		}
	return h;
}

auto getWordsFromFile(const string& filename) {
	unordered_map<wordHash, vector<string>> wordMap;
	ifstream infile{ filename };
	if (infile.good()) {
		string content{ istreambuf_iterator<char>(infile), istreambuf_iterator<char>() };
		string word;
		for (char c : content) {
			if (c == ',') {
				wordMap[getEliasDeltaHash64(word)].push_back(word);
				word.clear();
			}
			else if (c != '"') {
				word += c;
			}
		}
	}
	else {
		cout << "File not found. \n";
	}
	return wordMap;
}

auto getSquareMap(int digits) {
	vector<unordered_set<int>> map = { {} };
	for (int d = 1, f = 1, l; d <= digits; d++, f = l) {
		l = ceil(pow(10.0, d * .5));
		map.push_back({});
		for (int i = f; i < l; i++)
			map.back().insert(i*i);
	}
	return map;
}

auto compute() {
	auto wordMap = getWordsFromFile(".\\Data\\p098_words.txt");
	int maxWordLen = 0;
	vector<wordHash> anagrams;
	for (auto& pair : wordMap) {
		if (pair.second.size() > 1) {
			anagrams.push_back(pair.first);
			maxWordLen = max(maxWordLen, (int)pair.second.front().length());
		}
	}
	auto squareMap = getSquareMap(maxWordLen);
	int maxSquare = 0;
	for (auto& anagram : anagrams) {
		auto& words = wordMap[anagram];
		int wordLen = words.front().length();
		auto& squares = squareMap[wordLen];
		for (auto& square : squares) {
			for (int i = words.size() - 1; i > 0; i--) {
				vector<int> map(26, -1), digitMap(10, -1);
				bool duplicateDigit = false;
				for (int k = wordLen - 1, x = square; x; x /= 10, k--) {
					int digit = x % 10;
					int charIdx = words[i][k] - 'A';
					if ((map[charIdx] != -1 && map[charIdx] != digit)
						|| (digitMap[digit] != -1 && digitMap[digit] != charIdx)) {
						duplicateDigit = true;
						break;
					}
					map[charIdx] = digit;
					digitMap[digit] = charIdx;
				}
				if (duplicateDigit)
					continue;
				for (int j = i - 1; j >= 0; j--) {
					int num = accumulate(words[j].begin(), words[j].end(), 0, 
						[&](auto s, auto c){
							return s * 10 + map[c - 'A'];
						});;
					if (squares.find(num) != squares.end())
						maxSquare = max(maxSquare, max(square, num));
				}
			}
		}
	}
	return maxSquare;
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