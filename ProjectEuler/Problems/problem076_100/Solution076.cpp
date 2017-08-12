// https://projecteuler.net/problem=76
/*
It is possible to write five as a sum in exactly six different ways:

4 + 1
3 + 2
3 + 1 + 1
2 + 2 + 1
2 + 1 + 1 + 1
1 + 1 + 1 + 1 + 1

How many different ways can one hundred be written as a sum of at least two positive integers?

Solution:
The sequence is one less than "Partition Number" sequence.
p(n) = p(n-1) + p(n-2) - p(n-5) - p(n-7) + p(n-12) + p(n-15) - p(n-22) ...
Numbers are of the form m(3m - 1)/2, where m is an integer. 
The signs in the summation alternate as (-1)^(|m|-1)

*/

#include <iostream>
#include <vector>
#include <chrono>
using namespace std;


using num = unsigned;
constexpr int lim = 100;
auto compute() {
	vector<num> p = { 1 };
	p.reserve(lim);
	
	for (int i = 1; i <= lim; ++i) {
		// taking 2 pantagonal numbers as t1 and t2
		int t1 = 1, t2 = 2, d1 = 4, d2 = 5;
		num s = 0;
		do {
			s += p[i - t1];
			t1 += d1;
			d1 += 3;
			if (t2 > i)
				break;
			s += p[i - t2];
			t2 += d2;
			d2 += 3;
			// unrolling next 2 to avoid branch
			if (t1 > i)
				break;
			s -= p[i - t1];
			t1 += d1;
			d1 += 3;
			if (t2 > i)
				break;
			s -= p[i - t2];
			t2 += d2;
			d2 += 3;
		} while (t1 <= i);
		p.push_back(s);
	}

	return p.back();
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