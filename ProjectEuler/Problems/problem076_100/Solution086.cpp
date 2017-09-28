// https://projecteuler.net/problem=86
/*
Problem 86

Cuboid route

A spider, S, sits in one corner of a cuboid room, measuring 6 by 5 by 3, 
and a fly, F, sits in the opposite corner. By travelling on the surfaces of 
the room the shortest "straight line" distance from S to F is 10 
sqrt(6^2 + (5+3)^2) and the path is shown on the diagram.

    	+---------------------------------------+ F
       /|                                   .. /|
      / |                                ..   / |
     /  |                             ..     /  |
    /	|                          ..       /   | 3
   +----------------------------..---------+    |    
   |	|					..			   |    |
   |	+ - - - - - - -.. - - - - - - - - -|- - +
   |   /		  ..					   |   /
   |  /		  ..						   |  / 5
   | /   ..								   | /
   |/.             						   |/
 S +---------------------------------------+
                        6

However, there are up to three "shortest" path candidates for any given 
cuboid and the shortest route doesn't always have integer length.

It can be shown that there are exactly 2060 distinct cuboids, ignoring 
rotations, with integer dimensions, up to a maximum size of M by M by M, 
for which the shortest route has integer length when M = 100. This is the 
least value of M for which the number of solutions first exceeds two thousand; 
the number of solutions when M = 99 is 1975.

Find the least value of M such that the number of solutions first exceeds one million.

Solution:
First generate all the primitive pythagorean triplets.
Than use them to construct the cuboids.
Let x <= y <= z be a cuboid then (x+y)^2 + z^2 will always be the shortest.
Let a^2 + b^2 = c^2 is a pythagorean triplet
Then, we can either match
	a = x+y, b = z
	number of possible cuboids = a/2
or,
	a = z, b = x+y
	number of possible cuboids = a - (b + 1) / 2 + 1
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;

struct triplet {
	int a, b, c;
};

auto getPrimaryTriplets(int limit, bool sorted = false) {
	vector<triplet> triplets = { { 3,4,5 } };
	size_t approxSize = ceil(exp(log(limit) - exp(1) + 1.0));
	triplets.reserve(approxSize);

	for (int i = 0, a, b; i < triplets.size(); i++) {
		triplet &t = triplets[i];

		a = t.a * 1 - t.b * 2 + t.c * 2;
		b = t.a * 2 - t.b * 1 + t.c * 2;
		if (a <= limit && b <= limit)
			triplets.push_back({ a, b, t.a * 2 - t.b * 2 + t.c * 3 });

		a = -t.a * 1 + t.b * 2 + t.c * 2;
		b = -t.a * 2 + t.b * 1 + t.c * 2;
		if (a <= limit && b <= limit)
			triplets.push_back({ a, b, -t.a * 2 + t.b * 2 + t.c * 3 });

		a = t.a * 1 + t.b * 2 + t.c * 2;
		b = t.a * 2 + t.b * 1 + t.c * 2;
		if (a <= limit && b <= limit)
			triplets.push_back({ a, b, t.a * 2 + t.b * 2 + t.c * 3 });

		if (t.a > t.b)
			swap(t.a, t.b);
	}	

	if (sorted)
		sort(triplets.begin(), triplets.end(), [](auto &x, auto &y) {
			return x.b < y.b;
		});
	
	return triplets;
}

auto compute() {
	constexpr int limit = 2000;
	auto triplets = getPrimaryTriplets(2 * limit);
	
	vector<unsigned> counts(limit + 1, 0);
	for (auto& t : triplets) {
		if (t.a < 2 * t.b)	// breaking 'a' into 2 edges of cuboid
			for (int a = t.a, b = t.b; b <= limit; a += t.a, b += t.b)
				counts[b] += a / 2;
		if (t.b < 2 * t.a)	// breaking 'b' into 2 edges of cuboid
			for (int a = t.a, b = t.b; a <= limit; a += t.a, b += t.b)
				counts[a] += a - (b + 1) / 2 + 1;
	}

	int idx = 1;
	for (; counts[idx - 1] < 1'000'000 && idx <= limit; idx++)
		counts[idx] += counts[idx - 1];
	return idx <= limit ? idx - 1 : -1;
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