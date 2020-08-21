// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/dynamic-programming-set-4-longest-common-subsequence/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

auto longestCommonSubsequence(const string& seq1, const string& seq2) {
	int len1 = seq1.length(), len2 = seq2.length();
	vector<vector<int>> maxSoFar(len1 + 1, vector<int>(len2 + 1, 0));
	vector<string> path(len1 + 1, string(len2 + 1, ' '));
	
	for (int j = 1; j <= len1; j++)
		for (int i = 1; i <= len2; i++) {
			if(seq1[j-1] == seq2[i-1])
				path[j][i] = 'd', maxSoFar[j][i] = maxSoFar[j - 1][i - 1] + 1;
			else if (maxSoFar[j][i - 1] > maxSoFar[j - 1][i])
				path[j][i] = 'l', maxSoFar[j][i] = maxSoFar[j][i - 1];
			else
				path[j][i] = 'u', maxSoFar[j][i] = maxSoFar[j - 1][i];
		}

	string output;
	while (len1 && len2) {
		if (path[len1][len2] == 'u')
			--len1;
		else if (path[len1][len2] == 'l')
			--len2;
		else
			output.push_back(seq1[--len1]), --len2;
	}
	reverse(output.begin(), output.end());
	return output;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	const string s1 = "AGGTAB", s2 = "GXTXAYB";
	cout << longestCommonSubsequence(s1, s2) << '\n';
}