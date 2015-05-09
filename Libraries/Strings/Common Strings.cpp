#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

vector<string> split(const string &s, const string &delimiter){
	vector<string> tokens;
	size_t start = 0, index = 0;

	while ((index = s.find(delimiter, start)) != std::string::npos) {
		string token = s.substr(start, index - start);
		if (!token.empty())
			tokens.push_back(token);
		start = index + delimiter.length();
	}

	string token = s.substr(start, s.length() - start);
	if (!token.empty())
		tokens.push_back(token);

	return tokens;
}

vector<string> split(const string &s, const vector<char> &sortedDelimiters){
	vector<string> tokens;
	size_t start = 0, length = 0;

	for (auto c : s) {
		if (binary_search(sortedDelimiters.begin(), sortedDelimiters.end(), c)){
			if (length == 0){
				start++;
				continue;
			}
			tokens.push_back(s.substr(start, length));
			start += length + 1;
			length = 0;
		}
		else
			length++;
	}

	if (length != 0)
		tokens.push_back(s.substr(start, length));

	return tokens;
}

// recommended for large strings
vector<unsigned char> getDelimiterMapForSplit(const vector<unsigned char> &delimiters){
	vector<unsigned char> map(256, 0);

	for (auto c : delimiters){
		map[c] = 1;
	}

	return map;
}
vector<string> fastSplit(const string &s, const vector<unsigned char> &map){
	vector<string> tokens;
	size_t start = 0, length = 0;

	for (auto c : s) {
		if (map[c]){
			if (length == 0){
				start++;
				continue;
			}
			tokens.push_back(s.substr(start, length));
			start += length + 1;
			length = 0;
		}
		else
			length++;
	}

	if (length != 0)
		tokens.push_back(s.substr(start, length));

	return tokens;
}

int main(){
	string inputWithCommas;
	std::getline(cin, inputWithCommas);

	vector<string> tokens = split(inputWithCommas, ",");

	for (auto s : tokens)
		cout << s << endl;

#ifdef _MSC_VER
	std::system("pause");
#endif

	return 0;
}

// 1,2,3