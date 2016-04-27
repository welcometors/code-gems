#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;

struct edge { int a, b, c; edge(int x, int y, int z) : a(x), b(y), c(z) {} };

int main() {
	std::ios::sync_with_stdio(false);
	cin.tie(0);

	int v, e;
	cin >> v >> e;

	vector<edge> edges;
	edges.reserve(e);

	for (int i = 0, a, b, c; i < e; i++, edges.emplace_back(a - 1, b - 1, c))
		cin >> a >> b >> c;

	sort(edges.begin(), edges.end(), [](const edge& x, const edge& y) {
		return x.c < y.c;
	});

	int cost = 0;
	vector<unordered_set<int>*> sets(v, nullptr);
	for (auto &e : edges) {
		if (sets[e.a] == nullptr && sets[e.b] == nullptr) {
			auto s = new unordered_set<int>();
			s->insert(e.a);
			s->insert(e.b);
			sets[e.a] = sets[e.b] = s;
			cost += e.c;
			v -= 2;
		}
		else if (sets[e.a] == nullptr) {
			sets[e.b]->insert(e.a);
			sets[e.a] = sets[e.b];
			cost += e.c;
			v--;
		}
		else if (sets[e.b] == nullptr) {
			sets[e.a]->insert(e.b);
			sets[e.b] = sets[e.a];
			cost += e.c;
			v--;
		}
		else if (sets[e.a] != sets[e.b]) {
			auto min = (sets[e.a]->size() < sets[e.b]->size()) ? sets[e.a] : sets[e.b];
			auto max = (sets[e.a]->size() < sets[e.b]->size()) ? sets[e.b] : sets[e.a];
			for (auto &x : *min) {
				max->insert(x);
				sets[x] = max;
			}
			delete min;
			cost += e.c;
			v--;
		}
		if (!v)
			break;
	}

	delete sets[0];
	cout << cost << endl;
	return 0;
}

/*
4 5
1 2 10
1 3 24
2 3 2
2 4 15
3 4 7
*/