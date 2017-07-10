// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/dynamic-programming-set-5-edit-distance/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

auto editDistance(const string& seq1, const string& seq2) {
	int len1 = seq1.length(), len2 = seq2.length();
	vector<vector<int>> minSoFar(len1 + 1, vector<int>(len2 + 1));

	for (int j = 0; j <= len1; j++)
		minSoFar[j][0] = j;
	
	for (int i = 1; i <= len2; i++)
		minSoFar[0][i] = i;

	for (int j = 1; j <= len1; j++)
		for (int i = 1; i <= len2; i++) {
			if (seq1[j - 1] == seq2[i - 1])
				minSoFar[j][i] = minSoFar[j - 1][i - 1];
			else
				minSoFar[j][i] = min({ minSoFar[j - 1][i], minSoFar[j - 1][i - 1], minSoFar[j][i - 1] }) + 1;
		}

	return minSoFar.back().back();
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	const string s1 = "sunday", s2 = "saturday";
	cout << editDistance(s1, s2) << '\n';
}