// https://projecteuler.net/problem=33
/*
Digit cancelling fractions

The fraction 49/98 is a curious fraction, as an inexperienced mathematician in attempting to simplify
it may incorrectly believe that 49/98 = 4/8, which is correct, is obtained by cancelling the 9s.

We shall consider fractions like, 30/50 = 3/5, to be trivial examples.

There are exactly four non-trivial examples of this type of fraction, less than one in value, and
containing two digits in the numerator and denominator.

If the product of these four fractions is given in its lowest common terms, find the value of the denominator.

Solution:

*/

#include <iostream>
#include <cstdio>
#include <cmath>
#include <chrono>

typedef unsigned long long ull;
typedef long long ll;

typedef unsigned long natural;

typedef std::pair<int, int> frac;
frac& operator += (frac& l, const frac& r) {
	l.first += r.first;
	l.second += r.second;
	return l;
}
frac& operator *= (frac& l, const frac& r) {
	l.first *= r.first;
	l.second *= r.second;
	return l;
}
frac& reduce(frac& f) {
	for (int i = 2; i <= f.second; ++i) {
		while (f.first%i == 0 && f.second%i == 0) {
			f.first /= i;
			f.second /= i;
		}
	}
	return f;
}

constexpr int remove(int number, int place10) {
	return place10*(number / (place10 * 10)) + number % place10;
}

bool checkRecursive(const frac& ori, frac now, int k) {
	if (!k--)
		return now.second && ori.first*now.second == ori.second*now.first;

	for (int xd = now.first, xp = 1; xd; xp *= 10, xd /= 10) {
		int d = xd % 10;
		if (!d)	// cancelling 0 won't count
			continue;
		for (int yd = now.second, yp = 1; yd; yp *= 10, yd /= 10)
			if (d == yd % 10
				&& checkRecursive(ori, { remove(now.first, xp), remove(now.second, yp) }, k))
				return true;
	}
	return false;
}

inline bool check(const frac& n, int k) {
	return checkRecursive(n, n, k);
}

frac evaluate(int n, int k) {
	frac prod{ 1,1 };
	for (int start = pow(10, n - 1), i = start + 1; i < start * 10; i++)
		for (int j = start; j < i; j++)
			if (check({ j, i }, k)) {
				prod *= std::make_pair(j, i);
				//std::cout << j << '/' << i << '\n';
			}
	return prod;
}

int compute() {
	auto result = evaluate(2, 1);
	reduce(result);
	//std::cout << result.first << ", " << result.second << '\n';
	return result.second;
}

int main() {
	using namespace std;
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	cout << "Done in "
		<< duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1000000.0
		<< " miliseconds." << endl;
	cout << result << endl;
}