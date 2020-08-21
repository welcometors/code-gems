// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/dynamic-programming-set-12-longest-palindromic-subsequence/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

auto longestPalindromicSubsequence(const string& str) {
	int length = str.length();
	vector<vector<int>> opt(1, vector<int>(length + 1, 0));
	vector<string> ans;
	opt.push_back(vector<int>(length, 1));

	for (int len = 2; len <= length; len++) {
		vector<int> row;
		string rowT;
		row.reserve(length - len + 1);
		rowT.reserve(length - len + 1);

		for (int start = 0, end = len - 1; end < length; end++, start++) {
			if (str[start] == str[end]) {
				row.push_back(opt[len - 2][start + 1] + 2);
				rowT.push_back('e');
			}
			else if (opt[len - 1][start] >= opt[len - 1][start + 1]) {
				row.push_back(opt[len - 1][start]);
				rowT.push_back('l');
			}
			else {
				row.push_back(opt[len - 1][start + 1]);
				rowT.push_back('r');
			}
		}
		opt.emplace_back(row);
		ans.emplace_back(rowT);
	}

	string left;
	int l = 0, r = length - 1, level = ans.size();
	while (--level >= 0) {
		if (ans[level][l] == 'e') {
			left += str[l++];
			level--;
		}
		else if (ans[level][l] == 'l')
			r--;
		else
			l++;
	}

	string right = left;
	reverse(right.begin(), right.end());

	if (level == -1)
		left += str[l];

	return left + right;
}

auto longestPalindromicSubsequenceLength(const string& str) {
	int length = str.length();
	vector<vector<int>> opt(1, vector<int>(length + 1, 0));
	opt.push_back(vector<int>(length, 1));

	for (int len = 2; len <= length; len++) {
		vector<int> row;
		row.reserve(length - len + 1);
		for (int start = 0, end = len - 1; end < length; end++, start++)
			if (str[start] == str[end])
				row.push_back(opt[len - 2][start + 1] + 2);
			else
				row.push_back(max(opt[len - 1][start], opt[len - 1][start + 1]));
		opt.emplace_back(row);
	}

	return opt.back().back();
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	const string str = "BBABCBCAB";
	cout << longestPalindromicSubsequence(str) << '\n';
}