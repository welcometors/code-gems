// https://projecteuler.net/problem=62
/*
Cubic permutations

The cube, 41063625 (345^3), can be permuted to produce two other cubes:
56623104 (384^3) and 66430125 (405^3).
In fact, 41063625 is the smallest cube which has exactly three permutations of its
digits which are also cube.

Find the smallest cube for which exactly five permutations of its digits are cube.

Solution:
*/

#include <iostream>
#include <cstdint>
#include <unordered_map>
#include <algorithm>
#include <chrono>
using namespace std;
using u32 = uint32_t;
using u64 = uint64_t;

template<class T>
pair<T, u32> sortNumber(T n) {
	unsigned char dc[10] = {};
	u32 cnt = 0;
	while (n) {
		++dc[n % 10];
		n /= 10;
		++cnt;
	}
	for (u32 i = 9; i != -1; --i)
		for (auto t = dc[i]; t; --t)
			n = n * 10 + i;
	return{ n, cnt };
}

inline u64 cube(u64 n) {
	return n*n*n;
}

auto get(const u32 n, const u32 k) {
	unordered_map<u64, pair<u32, u32>> map;
	for (u32 i = 11, last = 4; i <= n; ++i) {
		auto key = sortNumber(cube(i));
		if (key.second > last) {
			u32 min = -1;
			for (const auto& rec : map)
				if (rec.second.first == k && rec.second.second < min)
					min = rec.second.second;
			if (min != -1)
				return min;
			map.clear();
			last = key.second;
		}
		auto result = map.find(key.first);
		if (result != map.end())
			++result->second.first;
		else
			map.insert(make_pair(key.first, make_pair(1, i)));
	}
	return 0U;
}

auto compute() {
	constexpr u32 limit = 10000;
	return cube(get(limit, 5));
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