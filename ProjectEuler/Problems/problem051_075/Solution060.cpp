// https://projecteuler.net/problem=60
/*
Prime pair sets

The primes 3, 7, 109, and 673, are quite remarkable. By taking any two primes and concatenating them in
any order the result will always be prime. For example, taking 7 and 109, both 7109 and 1097 are prime.
The sum of these four primes, 792, represents the lowest sum for a set of four primes with this property.

Find the lowest sum for a set of five primes for which any two primes concatenate to produce another prime.

Solution:
Create a graph for all the prime pair having the desired property.
Now, for each of them try to generate a group of 5 having the desired property in increasing order.
For each node: graph
Start with pair of 2s {node,neighbour} for all neightbours of node
now try to cobine into a set of pair of 3s, then 4s, then 5s
*/

#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <chrono>

namespace ForisekJancina32 {
	// ref  Fast Primality Testing for Integers That Fit into a Machine Word
	uint16_t bases[] = { 15591,2018,166,7429,8064,16045,10503,4399,1949,1295,2776,3620,560,3128,5212,
		2657,2300,2021,4652,1471,9336,4018,2398,20462,10277,8028,2213,6219,620,3763,4852,5012,3185,
		1333,6227,5298,1074,2391,5113,7061,803,1269,3875,422,751,580,4729,10239,746,2951,556,2206,
		3778,481,1522,3476,481,2487,3266,5633,488,3373,6441,3344,17,15105,1490,4154,2036,1882,1813,
		467,3307,14042,6371,658,1005,903,737,1887,7447,1888,2848,1784,7559,3400,951,13969,4304,177,41,
		19875,3110,13221,8726,571,7043,6943,1199,352,6435,165,1169,3315,978,233,3003,2562,2994,10587,
		10030,2377,1902,5354,4447,1555,263,27027,2283,305,669,1912,601,6186,429,1930,14873,1784,1661,
		524,3577,236,2360,6146,2850,55637,1753,4178,8466,222,2579,2743,2031,2226,2276,374,2132,813,
		23788,1610,4422,5159,1725,3597,3366,14336,579,165,1375,10018,12616,9816,1371,536,1867,10864,
		857,2206,5788,434,8085,17618,727,3639,1595,4944,2129,2029,8195,8344,6232,9183,8126,1870,3296,
		7455,8947,25017,541,19115,368,566,5674,411,522,1027,8215,2050,6544,10049,614,774,2333,3007,
		35201,4706,1152,1785,1028,1540,3743,493,4474,2521,26845,8354,864,18915,5465,2447,42,4511,
		1660,166,1249,6259,2553,304,272,7286,73,6554,899,2816,5197,13330,7054,2818,3199,811,922,350,
		7514,4452,3449,2663,4708,418,1621,1171,3471,88,11345,412,1559,194 };

	bool is_SPRP(uint32_t n, uint32_t a) {
		uint32_t d = n - 1, s = 0;
		while ((d & 1) == 0) ++s, d >>= 1;
		uint64_t cur = 1, pw = d;
		while (pw) {
			if (pw & 1) cur = (cur*a) % n;
			a = ((uint64_t)a*a) % n;
			pw >>= 1;
		}
		if (cur == 1) return true;
		for (uint32_t r = 0; r<s; r++) {
			if (cur == n - 1) return true;
			cur = (cur*cur) % n;
		}
		return false;
	}

	bool isPrime(uint32_t x) {
		if (x == 2 || x == 3 || x == 5 || x == 7) return true;
		if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0) return false;
		if (x<121)
			return x > 1;

		uint64_t h = x;
		h = ((h >> 16) ^ h) * 0x45d9f3b;
		h = ((h >> 16) ^ h) * 0x45d9f3b;
		h = ((h >> 16) ^ h) & 255;
		return is_SPRP(x, bases[h]);
	}
}

namespace Sieves {
	constexpr size_t compileTimeLogBase2(size_t n) {
		return (n <= 1) ? 0 : 1 + compileTimeLogBase2(n / 2);
	}

	constexpr size_t sizeOfElementInBytes = sizeof(uint32_t);
	constexpr size_t sizeOfElementInBits = 8 * sizeOfElementInBytes;
	constexpr size_t logSizeOfElementInBits = compileTimeLogBase2(sizeOfElementInBits);

	class PrimeSieve {
		std::unique_ptr<uint32_t[]> m_sieve;
	public:
		PrimeSieve(const PrimeSieve&) = default;
		PrimeSieve(PrimeSieve&&) = default;
		PrimeSieve& operator=(const PrimeSieve&) = default;
		PrimeSieve& operator=(PrimeSieve&&) = default;

		explicit PrimeSieve(const size_t minSieveSize) {
			const size_t numOfElementsInArray = 1 + minSieveSize / sizeOfElementInBits;
			m_sieve = std::make_unique<uint32_t[]>(numOfElementsInArray);
			const size_t limit = numOfElementsInArray*sizeOfElementInBytes;

			uint32_t* const sieve = m_sieve.get();
			std::memset(sieve, 0xAA, limit);
			sieve[0] ^= 6;

			const size_t sqrtLimit = std::sqrt(minSieveSize);
			for (size_t i = 3; i <= sqrtLimit; i += 2)
				if (sieve[i >> logSizeOfElementInBits] & (1 << (i&(sizeOfElementInBits - 1))))
					for (size_t j = i + i; j <= minSieveSize; j += i)
						sieve[j >> logSizeOfElementInBits] &= ~(1 << (j&(sizeOfElementInBits - 1)));
		}

		inline bool isPrime(size_t number) const {
			return m_sieve[number >> logSizeOfElementInBits] & (1 << (number&(sizeOfElementInBits - 1)));
		}

		auto countPrimes(const size_t minSieveSize) {
			const size_t numOfElementsInArray = 1 + minSieveSize / sizeOfElementInBits;
			uint32_t sum = 0;
			for (size_t i = 0; i < numOfElementsInArray; ++i)
				sum += _mm_popcnt_u32(m_sieve[i]);
			return sum;
		}
	};
}

template<class T>
T concat(const T a, const T b) {
	T r = a;
	T t = b;
	while (t) {
		r *= 10;
		t /= 10;
	}
	return r + b;
}

using EdgeList = std::vector<uint32_t>;
using Graph = std::unordered_map<uint32_t, EdgeList>;

constexpr uint32_t limit = 10'000, minConnectivity = 5;
Sieves::PrimeSieve sieve(limit);

inline bool isPrime(uint32_t n) {
	if (n <= limit)
		return sieve.isPrime(n);
	return ForisekJancina32::isPrime(n);
}

inline bool isPrimePair(uint32_t a, uint32_t b) {
	return isPrime(concat(a, b)) && isPrime(concat(b, a));
}

auto getPrimeGraph() {
	std::vector<uint32_t> primes = { 2 };
	primes.reserve(sieve.countPrimes(limit));
	for (size_t i = 3; i < limit; i += 2)
		if (sieve.isPrime(i))
			primes.push_back(i);

	Graph graph;
	for (size_t i = 1, n = primes.size(); i < n; ++i) {
		EdgeList *list = nullptr;
		for (size_t j = 0; j < i; ++j) {
			if (isPrimePair(primes[i], primes[j])) {
				if (list == nullptr)
					list = &(graph.insert(make_pair(primes[i], EdgeList({ primes[j] }))).first->second);
				else
					list->push_back(primes[j]);
			}
		}
		if (list != nullptr)
			std::sort(list->begin(), list->end());
	}

	return graph;
}

auto getMaxConSum(Graph &graph, uint32_t prime, uint32_t minConnectivity) {
	auto& neighbourList = graph[prime];
	std::vector<EdgeList> groups;
	for (const auto& neighbour : neighbourList)
		groups.push_back(EdgeList({ prime,neighbour }));

	for (uint32_t connectivity = 2; connectivity < minConnectivity && groups.size(); ++connectivity) {
		if (groups.size() < connectivity) {
			groups.clear(); // groups should have n groups to make a n+1 group 
			break;
		}

		std::vector<EdgeList> newGroups;
		for (const auto& group : groups) {
			const auto& least = group.back();
			for (const auto& neighbour : neighbourList) {
				if (neighbour >= least)
					break;
				bool flag = true;
				for (const auto& val : group) {
					if (!std::binary_search(std::begin(graph[val]), std::end(graph[val]), neighbour)) {
						flag = false;
						break;
					}
				}
				if (flag) {
					EdgeList newGroup = group;
					newGroup.push_back(neighbour);
					newGroups.emplace_back(newGroup);
				}
			}
		}
		groups = std::move(newGroups);
	}

	uint32_t min = ~0;
	for (const auto& group : groups) {
		uint32_t sum = 0;
		for (const auto& val : group)
			sum += val;
		if (min > sum)
			min = sum;
	}
	return min;
}

auto compute() {
	auto graph = getPrimeGraph();

	uint32_t min = ~0;
	for (auto& node : graph) {
		if (node.second.size() >= minConnectivity - 1) {
			auto minSum = getMaxConSum(graph, node.first, minConnectivity);
			if (min > minSum)
				min = minSum;
		}
	}
	return min;
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