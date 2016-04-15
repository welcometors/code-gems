#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>
using namespace std;

const vector<vector<char>> giveTable = { {}, {}, {}, {}, {}, {}, {'5'}, {'1'}, {'0', '6', '9'}, {'3','5'} };
const vector<vector<char>> takeTable = { {'8'}, {'7'}, {}, {'9'}, {}, {'6','9'}, {'8'}, {}, {}, {'8'} };
const vector<vector<char>> rargTable = { {'6', '9'}, {}, {'3'}, {'2', '5'}, {}, {'3'}, {'0','9'}, {}, {}, {'0','6'} };
enum class OperationType{Give, Take, Rearrange};

unordered_set<int> getArrangements(const string &num, OperationType op) {
	unordered_set<int> outcomes;

	switch (op) {
	case OperationType::Give:
		for (int i = 0, n = num.length(); i < n; i++) {
			string s(num);
			for (const auto c : giveTable[num[i] - '0']) {
				if (!i && c == '0') continue; // leading zeroes not allowed
				s[i] = c;
				outcomes.insert(stoi(s));
			}
		}
		return outcomes;

	case OperationType::Take:
		for (int i = 0, n = num.length(); i < n; i++) {
			string s(num);
			for (const auto c : takeTable[num[i] - '0']) {
				if (!i && c == '0') continue; // leading zeroes not allowed
				s[i] = c;
				outcomes.insert(stoi(s));
			}
		}
		return outcomes;
	}

	for (int i = 0, n = num.length(); i < n; i++) {
		for (int j = 0; j < n; j++) {
			string s(num);
			if (i == j) {
				for (const auto c : rargTable[num[i] - '0']) {
					if (!i && c == '0') continue; // leading zeroes not allowed
					s[i] = c;
					outcomes.insert(stoi(s));
				}
			}
			else {
				// take from ith and give it to jth
				for (const auto c : takeTable[num[i] - '0']) {
					if (!i && c == '0') continue; // leading zeroes not allowed
					s[i] = c;
					for (const auto d : giveTable[num[j] - '0']) {
						if (!j && d == '0') continue; // leading zeroes not allowed
						s[j] = d;
						outcomes.insert(stoi(s));
					}
				}
			}
		}
	}

	return outcomes;
}

int MatchstickSolutions(std::string equation) {
	char op, eq;
	int a, b, c;
	stringstream ss(equation);
	ss >> a >> op >> b >> eq >> c;
	string as(to_string(a)), bs(to_string(b)), cs(to_string(c));

	int sols = 0;
	auto aarg = getArrangements(as, OperationType::Rearrange);
	if (aarg.find((op == '+') ? c - b : c + b) != aarg.end())
		sols++;

	auto barg = getArrangements(bs, OperationType::Rearrange);
	if (barg.find((op == '+') ? c - a : a - c) != barg.end())
		sols++;

	auto carg = getArrangements(cs, OperationType::Rearrange);
	if (carg.find((op == '+') ? a + b : a - b) != carg.end())
		sols++;

	auto agive = getArrangements(as, OperationType::Give);
	auto bgive = getArrangements(bs, OperationType::Give);
	auto cgive = getArrangements(cs, OperationType::Give);
	auto atake = getArrangements(as, OperationType::Take);
	auto btake = getArrangements(bs, OperationType::Take);
	auto ctake = getArrangements(cs, OperationType::Take);
	
	if (op == '-' && agive.find(c - b) != agive.end())
		sols++;
	if (op == '-' && bgive.find(c - a) != bgive.end())
		sols++;
	if (op == '-' && cgive.find(a + b) != cgive.end())
		sols++;

	if (op == '+' && atake.find(c + b) != atake.end())
		sols++;
	if (op == '+' && btake.find(a - c) != btake.end())
		sols++;
	if (op == '+' && ctake.find(a - b) != ctake.end())
		sols++;

	for (auto& x : agive) {
		if (btake.find((op == '+') ? c - x : x - c) != btake.end())
			sols++;
		if (ctake.find((op == '+') ? x + b : x - b) != ctake.end())
			sols++;
	}

	for (auto& x : bgive) {
		if (atake.find((op == '+') ? c - x : c + x) != atake.end())
			sols++;
		if (ctake.find((op == '+') ? a + x : a - x) != ctake.end())
			sols++;
	}

	for (auto& x : cgive) {
		if (atake.find((op == '+') ? x - b : x + b) != atake.end())
			sols++;
		if (btake.find((op == '+') ? x - a : a - x) != btake.end())
			sols++;
	}

	return sols;
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);

	cout << MatchstickSolutions("10+1=3") << endl;
	cout << MatchstickSolutions("1-61=0") << endl;
	cout << MatchstickSolutions("7+1=8") << endl;

	//for( auto x: getArrangements("61", OperationType::Rearrange))
	//	cout << x << endl;
}
