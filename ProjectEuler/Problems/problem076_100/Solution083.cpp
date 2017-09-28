// https://projecteuler.net/problem=83
/*
Path sum: four ways

In the 5 by 5 matrix below, the minimal path sum from the top left to the 
bottom right, by moving left, right, up, and down, is indicated in bold 
red and is equal to 2297.

[131]  673  [234] [103]  [18]
[201]  [96] [342]  965  [150]
 630   803   746  [422] [111]
 537   699   497  [121]  956
 805   732   524   [37] [331]

Find the minimal path sum, in matrix.txt (right click and "Save Link/Target As..."), 
a 31K text file containing a 80 by 80 matrix, from the top left to the bottom 
right by only moving right and down.

Solution:

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <chrono>
using namespace std;

using natural = unsigned long long;

template<class T>
using matrixRow = vector<T>;

template<class T>
using matrix = vector<matrixRow<T>>;

auto fastSplit(const string &s) {
	matrixRow<unsigned> tokens;
	size_t start = 0, length = 0;

	for (auto c : s) {
		if (c == ',') {
			if (length == 0) {
				start++;
				continue;
			}
			tokens.push_back(stoi(s.substr(start, length)));
			start += length + 1;
			length = 0;
		}
		else
			length++;
	}

	if (length != 0)
		tokens.push_back(stoi(s.substr(start, length)));

	return tokens;
}

auto getMatrix(const string &fileName) {
	matrix<unsigned> x;
	std::ifstream file(fileName);
	if (file.good()) {
		while (!file.eof()) {
			string row;
			file >> row;
			auto elems = fastSplit(row);
			if (elems.size())
				x.emplace_back(elems);
		}
	}
	else {
		cout << "File not found. \n";
	}
	return x;
}

struct vertex {
	size_t y;
	size_t x;
	natural v;
};

class vertexCompare {
public:
	bool operator() (const vertex* a, const vertex *b) const {
		if (a->v != b->v)
			return a->v < b->v;
		if (a->y != b->y)
			return a->y < b->y;
		return a->x < b->x;
	}
};

inline void checkNeighbour(vertex &v, const vertex &u, const matrix<unsigned>& mat, set<vertex*, vertexCompare> &q) {
	natural nd = u.v + mat[v.y][v.x];
	if (nd < v.v) {
		q.erase(&v);
		v.v = nd;
		q.insert(&v);
	}
}

auto getMinimumSum(const matrix<unsigned>& mat) {
	const size_t n = mat.size();
	matrix<vertex> map(n, matrixRow<vertex>(n));

	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < n; j++)
			map[i][j] = { i, j, natural(0) - 1 };
	map[n - 1][n - 1].v = mat[n - 1][n - 1];

	set<vertex*, vertexCompare> pQueue{ &(map[n - 1][n - 1]) };
	matrix<bool> visited(n, matrixRow<bool>(n, false));

	while (pQueue.size()) {
		vertex &u = **pQueue.begin();
		pQueue.erase(pQueue.begin());
		visited[u.y][u.x] = true;

		if (u.y > 0 && !visited[u.y - 1][u.x])
			checkNeighbour(map[u.y - 1][u.x], u, mat, pQueue);
		if (u.x > 0 && !visited[u.y][u.x - 1])
			checkNeighbour(map[u.y][u.x - 1], u, mat, pQueue);
		if (u.y < n - 1 && !visited[u.y + 1][u.x])
			checkNeighbour(map[u.y + 1][u.x], u, mat, pQueue);
		if (u.x < n - 1 && !visited[u.y][u.x + 1])
			checkNeighbour(map[u.y][u.x + 1], u, mat, pQueue);
	}

	return map.front().front().v;
}

auto compute() {
	/*matrix<unsigned> x = {
		{ 131, 673, 234, 103,  18 },
		{ 201,  96, 342, 965, 150 },
		{ 630, 803, 746, 422, 111 },
		{ 537, 699, 497, 121, 956 },
		{ 805, 732, 524,  37, 331 }
	};
	return getMinimumSum(x);*/
	return getMinimumSum(getMatrix(".\\Data\\p081_matrix.txt"));
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