// https://projecteuler.net/problem=79
/*
Passcode derivation

A common security method used for online banking is to ask the user for
three random characters from a passcode. For example, if the passcode was
531278, they may ask for the 2nd, 3rd, and 5th characters;
the expected reply would be: 317.

The text file, keylog.txt, contains fifty successful login attempts.

Given that the three characters are always asked for in order, analyse the
file so as to determine the shortest possible secret passcode of unknown length.

Solution:
Since the digits are not repeated. We can use topological sort to obtain the sequence.

*/

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <chrono>
using namespace std;

const vector<string> pairs = {
	"319", "680", "180", "690", "129", "620", "762", "689", "762", "318", "368", "710",
	"720", "710", "629", "168", "160", "689", "716", "731", "736", "729", "316", "729",
	"729", "710", "769", "290", "719", "680", "318", "389", "162", "289", "162", "718",
	"729", "319", "790", "680", "890", "362", "319", "760", "316", "729", "380", "319",
	"728", "716"
};

using graph = map<char, set<char>>;
auto compute() {
	graph g, gr;
	set<char> s;
	for (auto& p : pairs) {
		g[p[1]].insert(p[0]);
		g[p[2]].insert(p[1]);
		s.insert(p[0]);
		s.insert(p[1]);
		s.insert(p[2]);
	}
	string o;
	o.reserve(s.size());

	for (auto& e : g)
		s.erase(e.first);

	while (s.size()) {
		char v = *s.begin();
		s.erase(v);
		o += v;
		vector<char> r;
		for (auto& e : g)
			if (e.second.erase(v) == 1 && e.second.size() == 0)
				r.push_back(e.first);
		for (auto& u : r)
			s.insert(u), g.erase(u);
	}

	return o;
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