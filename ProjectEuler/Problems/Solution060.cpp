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

*/

#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
using namespace std;
typedef unsigned long long natural;

vector<int> populatePrimes(int limit) {
	vector<int> primes;
	int sqrtLimit = (int)sqrt(limit + 1) + 1, k = 0;
	unique_ptr<int[]> isPrime(new int[limit + 1]);

	isPrime[0] = isPrime[1] = 0;
	std::memset(&isPrime[2], 1, sizeof(int)*(limit - 1));
	primes.clear();

	for (int i = 2; i <= sqrtLimit; i++) {
		if (!isPrime[i])
			continue;

		primes.push_back(i);

		for (int j = i + i; j <= limit; j += i)
			isPrime[j] = false;
	}

	for (int i = sqrtLimit + 1; i <= limit; i++) {
		if (isPrime[i])
			primes.push_back(i);
	}

	return primes;
}

bool isPrime(const vector<int>& primes, int n) {
	for (int i = 0; primes[i] <= sqrt(n); i++)
		if (n % primes[i] == 0)
			return false;
	return true;
}

int concat(int a, int b) {
	int c = b;
	while (c) {
		a *= 10;
		c /= 10;
	}
	return a + b;
}

inline bool check(const vector<int>& primes, int a, int b) {
	return isPrime(primes, concat(a, b)) && isPrime(primes, concat(b, a));
}

struct SetHash {
	size_t operator() (const unordered_set<int> &data) const {
		size_t sum = 0;
		for (auto v : data)
			sum += v;
		return sum;
	}
};

unsigned getMaxConSum(unordered_map<int, unordered_set<int>>& graph, int idx, const vector<int>& primes, int minConnectivity) {
	unordered_set<unordered_set<int>, SetHash> groups;
	for (auto i : graph[idx])
		groups.insert(unordered_set<int>({ idx,i }));

	for (int connectivity = 2; connectivity < minConnectivity && groups.size() != 0; connectivity++) {
		unordered_set<unordered_set<int>, SetHash> newGroups;
		for (auto& group : groups) {
			for (auto newVal : graph[idx]) {
				if (group.find(newVal) == group.end()) {
					bool flag = true;
					for (auto val : group) {
						if (graph[val].find(newVal) == graph[val].end()) {
							flag = false;
							break;
						}
					}
					if (flag) {
						unordered_set<int> newGroup = group;
						newGroup.insert(newVal);
						newGroups.insert(newGroup);
					}
				}
			}
		}
		groups = std::move(newGroups);
	}

	unsigned min = -1;
	for (auto& group : groups) {
		unsigned sum = 0;
		for (auto v : group)
			sum += primes[v];
		if (sum < min)
			min = sum;
	}

	return min;
}

int main() {
	const natural limit = 10000;
	const int minConnectivity = 5;
	auto primes = populatePrimes(2 * limit);

	int primeLimit = 0;
	while (primes[primeLimit] <= limit)
		primeLimit++;

	unordered_map<int, unordered_set<int>> graph;
	for (int i = 1; i < primeLimit; i++)
		for (int j = 0; j < i; j++)
			if (check(primes, primes[i], primes[j])) {
				if (graph.find(i) != graph.end())
					graph[i].insert(j);
				else
					graph[i] = unordered_set<int>({ j });
			}

	unsigned min = -1;
	for (auto& node : graph) {
		if (node.second.size() >= minConnectivity) {
			unsigned sum = getMaxConSum(graph, node.first, primes, minConnectivity);
			if (sum < min)
				min = sum;
		}
	}
	cout << min << endl;
}