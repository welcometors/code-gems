// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/length-of-the-longest-substring-without-repeating-characters/

#include <iostream>
#include <string>
#include <vector>
using namespace std;

auto maxNonRepeatingSubstring(const string& s) {
	vector<int> indexMap(128, -1);
	vector<int> lastIndex(s.size());
	for (int i = 0; i < s.size(); i++) {
		lastIndex[i] = indexMap[s[i]];
		indexMap[s[i]] = i;
	}

	int bestStart = 0, bestLength = 1;
	int start = 0;
	for (int end = 1; end < s.size(); end++) {
		if (start <= lastIndex[end])
			start = lastIndex[end] + 1;

		int currentLength = end - start + 1;
		if (currentLength > bestLength) {
			bestStart = start;
			bestLength = currentLength;
		}
	}

	return s.substr(bestStart, bestLength);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	string s = "geeksforgeeks";
	cout << maxNonRepeatingSubstring(s) << endl;
}