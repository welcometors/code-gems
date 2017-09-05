// https://projecteuler.net/problem=55
/*
Lychrel numbers

If we take 47, reverse and add, 47 + 74 = 121, which is palindromic.

Not all numbers produce palindromes so quickly. For example,

349 + 943 = 1292,
1292 + 2921 = 4213
4213 + 3124 = 7337

That is, 349 took three iterations to arrive at a palindrome.

Although no one has proved it yet, it is thought that some numbers,
like 196, never produce a palindrome. A number that never forms a palindrome
through the reverse and add process is called a Lychrel number. Due to the
theoretical nature of these numbers, and for the purpose of this problem,
we shall assume that a number is Lychrel until proven otherwise. In addition
you are given that for every number below ten-thousand,
it will either
(i) become a palindrome in less than fifty iterations, or,
(ii) no one, with all the computing power that exists, has managed so far to
map it to a palindrome. In fact, 10677 is the first number to be shown to
require over fifty iterations before producing a palindrome:
4668731596684224866951378664 (53 iterations, 28-digits).

Surprisingly, there are palindromic numbers that are themselves Lychrel numbers;
the first example is 4994.

How many Lychrel numbers are there below ten-thousand?

NOTE: Wording was modified slightly on 24 April 2007 to emphasise the theoretical
nature of Lychrel numbers.

Solution:

*/

#include <iostream>
#include <cstdint>
#include <string>
#include <chrono>

template<class T>
T reverse(T n) {
	T r = 0;
	while (n) {
		r = r * 10 + n % 10;
		n /= 10;
	}
	return r;
}

constexpr uint32_t cutoff = 50;
auto isLychrelStr(std::string& n, const uint32_t iter) {
	if (iter == cutoff)
		return iter;

	auto r = n;
	std::reverse(r.begin(), r.end());
	if (n == r)
		return iter;

	std::string sum;
	int c = 0;
	for (size_t i = 0, l = n.length(); i < l; ++i) {
		c += n[i] - '0' + r[i] - '0';
		sum += (char)('0' + c % 10);
		c /= 10;
	}
	if (c)
		sum += (char)('0' + c);
	std::reverse(sum.begin(), sum.end());
	n = std::move(sum);

	return isLychrelStr(n, iter + 1);
}

auto isLychrel64(const uint64_t n, const uint32_t iter) {
	if (iter == cutoff)
		return iter;

	const auto r = reverse(n);
	if (n == r)
		return iter;

	if (n < (1ULL << 60))
		return isLychrel64(n + r, iter + 1);

	auto n_r = std::to_string(n + r);
	return isLychrelStr(n_r, iter + 1);
}

auto compute() {
	constexpr uint32_t limit = 10000;

	uint32_t count[cutoff + 1] = {};
	for (uint32_t n = 1; n < limit; ++n)
		++count[isLychrel64(n + reverse(n), 1)];

	// Statistics
	/*for (size_t i = 0, s = count[i]; i <= cutoff; s += count[++i])
	std::cout << i << ':' << count[i] << '|' << 100.0*s / limit << '\n';*/

	return count[cutoff];
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
