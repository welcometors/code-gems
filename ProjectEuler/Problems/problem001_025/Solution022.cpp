// https://projecteuler.net/problem=22
/*
Names scores

Using names.txt (right click and 'Save Link/Target As...'), a 46K text file containing over five-thousand 
first names, begin by sorting it into alphabetical order. Then working out the alphabetical value for 
each name, multiply this value by its alphabetical position in the list to obtain a name score.

For example, when the list is sorted into alphabetical order, COLIN, which is worth 
3 + 15 + 12 + 9 + 14 = 53, is the 938th name in the list. 
So, COLIN would obtain a score of 938 × 53 = 49714.

What is the total of all the name scores in the file?
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

int main() {
	vector<string> names;
	ifstream file(".\\Data\\p022_names.txt");
	if(file.good()){
		string text, name;
		file >> text;
		text += ',';
		for (char c : text) {
			if (c == ',') {
				names.emplace_back(name);
				name.clear();
			}
			else if (c != '"')
				name += c;
		}
	}
	else {
		cout << "File not found" << endl;
	}

	sort(names.begin(), names.end());

	int sum = 0, i = 1;
	for (auto& name : names) {
		int score = 0;
		for (char c : name)
			score += c - 'A' + 1;
		sum += score * i++;
	}
	cout << sum << '\n';
}
