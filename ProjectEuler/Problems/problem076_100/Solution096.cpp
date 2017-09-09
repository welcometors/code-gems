// https://projecteuler.net/problem=96
/*
Problem 96

Su Doku

Su Doku (Japanese meaning number place) is the name given to a popular puzzle concept. 
Its origin is unclear, but credit must be attributed to Leonhard Euler who invented 
a similar, and much more difficult, puzzle idea called Latin Squares. The objective 
of Su Doku puzzles, however, is to replace the blanks (or zeros) in a 9 by 9 grid in 
such that each row, column, and 3 by 3 box contains each of the digits 1 to 9. 
Below is an example of a typical starting puzzle grid and its solution grid.

	0 0 3 | 0 2 0 | 6 0 0
	9 0 0 | 3 0 5 | 0 0 1
	0 0 1 | 8 0 6 | 4 0 0
	------+-------+------
	0 0 8 | 1 0 2 | 9 0 0
	7 0 0 | 0 0 0 | 0 0 8
	0 0 6 | 7 0 8 | 2 0 0
	------+-------+------
	0 0 2 | 6 0 9 | 5 0 0
	8 0 0 | 2 0 3 | 0 0 9
	0 0 5 | 0 1 0 | 3 0 0

	4 8 3 | 9 2 1 | 6 5 7
	9 6 7 | 3 4 5 | 8 2 1
	2 5 1 | 8 7 6 | 4 9 3
	------+-------+------
	5 4 8 | 1 3 2 | 9 7 6
	7 2 9 | 5 6 4 | 1 3 8
	1 3 6 | 7 9 8 | 2 4 5
	------+-------+------
	3 7 2 | 6 8 9 | 5 1 4
	8 1 4 | 2 5 3 | 7 6 9
	6 9 5 | 4 1 7 | 3 8 2

A well constructed Su Doku puzzle has a unique solution and can be solved by logic, 
although it may be necessary to employ "guess and test" methods in order to eliminate 
options (there is much contested opinion over this). The complexity of the search 
determines the difficulty of the puzzle; the example above is considered easy because 
it can be solved by straight forward direct deduction.

The 6K text file, sudoku.txt (right click and 'Save Link/Target As...'), contains 
fifty different Su Doku puzzles ranging in difficulty, but all with unique solutions 
(the first puzzle in the file is the example above).

By solving all fifty puzzles find the sum of the 3-digit numbers found in the top 
left corner of each solution grid; for example, 483 is the 3-digit number found in 
the top left corner of the solution grid above.

Solution:
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;

namespace sudoku {
	using namespace std;

#ifdef _MSC_VER
#include <intrin.h>
#include <cstdint>
	uint32_t __inline __builtin_clz(uint32_t value) {
		unsigned long leading_zero = 0;
		if (_BitScanReverse(&leading_zero, value))
			return 31 - leading_zero;
		return 32;
	}
#endif

	class sudoku9x9 {
		vector<uint32_t> elements;

	public:
		sudoku9x9() : elements(81, 0b111'111'111'0) {}

		sudoku9x9(const sudoku9x9&) = default;
		sudoku9x9(sudoku9x9&&) = default;
		sudoku9x9& operator=(const sudoku9x9&) = default;
		sudoku9x9& operator=(sudoku9x9&&) = default;

		inline uint32_t putElement(size_t i, uint32_t v) {
			if (v && v < 10) {
				const uint32_t m = 1 << v; // mask
				if (elements[i] & m) {
					// horizontal remove
					for (size_t j = 9 * (i / 9), k = 0; k < 9; k++, j++)
						elements[j] &= ~m;
					// vertical remove
					for (size_t j = i % 9; j < 81; j += 9)
						elements[j] &= ~m;
					// box remove
					for (size_t j = 27 * ((i / 9) / 3) + 3 * (i % 9 / 3), k = 0; k < 3; k++, j += 6)
						for (size_t l = 0; l < 3; l++, j++)
							elements[j] &= ~m;
					return elements[i] = m | 1;
				}
			}
			return 0;
		}

		inline uint32_t getElement(size_t i) const {
			return (elements[i] & 1)? 31 - __builtin_clz(elements[i]): 0;
		}

		// maxDepth = 2 is enough, even for very hard sudokus
		int solve(int maxDepth = 2) {
			if (!maxDepth)
				return -1;

			int changes = 0, iterations = 0;
			do{
				changes = 0;
				// single possibility
				for (int i = 0; i < 81; i++) {
					if (!(elements[i] & (elements[i] - 1))) {
						if(!putElement(i, 31 - __builtin_clz(elements[i])))
							return -1;
						changes++;
					}
				}
				if (changes) {
					iterations += 1;
					continue;
				}
				// horizontal fill
				for (int i = 0; i < 81; ) {
					uint64_t count = 0, position = 0;
					for (size_t j = 0; j < 9; j++, i++)
						if (!(elements[i] & 1))
							for (uint32_t x = elements[i] >> 1, k = 0; x; x >>= 1, k += 7)
								if (x & 1) {
									position &= ~(uint64_t(127) << k);
									position |= uint64_t(i) << k;
									count += uint64_t(1) << k;
								}
					for (uint32_t j = 1; count; count >>= 7, position >>= 7, j++)
						if ((count & 127) == 1) {
							if (!putElement(position & 127, j))
								return -1;
							changes++;
						}
				}
				if (changes) {
					iterations += 2;
					continue;
				}
				// vertical fill
				for (int i = 0; i < 9; i++) {
					uint64_t count = 0, position = 0;
					for (size_t j = i; j < 81; j += 9)
						if (!(elements[j] & 1))
							for (uint32_t x = elements[j] >> 1, k = 0; x; x >>= 1, k += 7)
								if (x & 1) {
									position &= ~(uint64_t(127) << k);
									position |= uint64_t(j) << k;
									count += uint64_t(1) << k;
								}
					for (uint32_t j = 1; count; count >>= 7, position >>= 7, j++)
						if ((count & 127) == 1) {
							if (!putElement(position & 127, j))
								return -1;
							changes++;
						}
				}
				if (changes) {
					iterations += 3;
					continue;
				}
				// box fill
				for (int by = 0; by < 81; by += 27)
					for (int bx = 0; bx < 9; bx += 3) {
						uint64_t count = 0, position = 0;
						for (size_t i = 0; i < 27; i += 9)
							for (size_t j = 0, p = by + bx + i; j < 3; j++, p++)
								if (!(elements[p] & 1))
									for (uint32_t x = elements[p] >> 1, k = 0; x; x >>= 1, k += 7)
										if (x & 1) {
											position &= ~(uint64_t(127) << k);
											position |= uint64_t(p) << k;
											count += uint64_t(1) << k;
										}
						for (uint32_t j = 1; count; count >>= 7, position >>= 7, j++)
							if ((count & 127) == 1) {
								if (!putElement(position & 127, j))
									return -1;
								changes++;
							}
					}
				iterations += 4;
			} while (changes);

			for (int i = 0; i < 81; i++)
				if (!elements[i])
					return -1;

			for (int i = 0; i < 81; i++)
				if (!(elements[i] & 1)) {
					vector<int> possibleValues;
					uint32_t x = elements[i] >> 1;
					for (int k = 1; x; x >>= 1, k++)
						if(x & 1)
							possibleValues.push_back(k);
					for (auto& value : possibleValues) {
						sudoku9x9 s(*this);
						s.putElement(i, value);
						auto result = s.solve(maxDepth - 1);
						if (result != -1) {
							*this = s;
							return result + iterations + 1;
						}
					}
				}

			for (int i = 0; i < 81; i++)
				if (!(elements[i] & 1))
					return -1;

			return iterations;
		}

		void showPossiblities() {
			int ary[27][27];
			for (int r = 0, i = 0; r < 9; r++)
				for (int c = 0; c < 9; c++, i++)
					for (int y = 0, v = elements[i] / 2, t = 1; y < 3; y++)
						for (int x = 0; x < 3; x++, v /= 2, t++)
							ary[r * 3 + y][c * 3 + x] = v % 2 ? t : 0;

			for (int r = 0; r < 27; r++){
				for (int c = 0; c < 27; c++) {
					if(ary[r][c])
						cout << ary[r][c];
					else
						cout << ' ';
					if (c % 3 == 2)
						cout << ' ';
					if (c != 26 && c % 9 == 8)
						cout << "|| ";
					else if (c != 26 && c % 3 == 2)
						cout << "| ";
				}
				cout << '\n';
				if (r != 26 && r % 9 == 8)
					cout << string(53, '=') << '\n';
				else if (r != 26 && r % 3 == 2)
					cout << string(53, '-') << '\n';
			}
		}

		friend istream &operator >> (istream &input, sudoku9x9& s);
		friend ostream &operator << (ostream &output, const sudoku9x9& s);
	};

	istream &operator >> (istream& input, sudoku9x9& s) {
		string line;
		for (uint32_t i = 0; i<81; ) {
			input >> line;
			for (auto& c : line)
				s.putElement(i++, c - '0');
		}
		return input;
	}

	ostream &operator << (ostream &output, const sudoku9x9& s) {
		for (int r = 0, i = 0; r < 9; r++) {
			for (int c = 0; c < 9; c++, i++) {
				output << ' ' << s.getElement(i) << ' ';
				if (c == 2 || c == 5)
					output << '|';
			}
			output << '\n';
			if (r == 2 || r == 5)
				output << "---------+---------+---------" << '\n';
		}
		return output;
	}
}

auto compute() {
	ifstream infile{ ".\\Data\\p096_sudoku.txt" };
	int sum = 0;
	if (infile.good()) {
		string line;
		while (getline(infile, line)) {
			//cout << line << '\n';
			sudoku::sudoku9x9 s;
			infile >> s;
			infile.ignore();
			if (s.solve() == -1) {
				s.showPossiblities();
				break;
			}
			sum += s.getElement(0) * 100 + s.getElement(1) * 10 + s.getElement(2);
		}
	}
	else {
		cout << "File not found. \n";
	}
	return sum;
}

template <class T>
inline void DoNotOptimize(const T &value) {
	__asm { lea ebx, value }
}

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
