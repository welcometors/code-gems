#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#pragma region "ANAGRAM"
// USES: unordered_map<AlphabetCounter<26, 'a'>, int, AlphabetCounterHasher<26, 'a'>> substring;

template <size_t n, size_t base>
struct AlphabetCounter{
	int count[n];

	AlphabetCounter(const string &s, int start, int end){
		fill(count, count + n, 0);
		for (int i = start; i <= end; i++)
			count[s[i] - base]++;
	}

	bool operator == (const AlphabetCounter &other) const{
		for (int i = 0; i < n; i++)
			if (count[i] != other.count[i])
				return false;
		return true;
	}
};

template <size_t n, size_t base>
struct AlphabetCounterHasher
{
	std::size_t operator()(const AlphabetCounter<n, base>& k) const
	{
		size_t hash = 0;
		for (auto x : k.count)
			hash = (hash + (324723947 + x)) ^ 93485734985;
		return hash;
	}
};

#pragma endregion

#pragma region "SPLIT"

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

#pragma endregion

std::string replaceAll(const std::string &str, const std::string& oldString, const std::string& newString) {
	std::string out;
	size_t startPos = 0, findIdx, templeteLength = oldString.length();
	while ((findIdx = str.find(oldString, startPos)) != std::string::npos) {
		out += str.substr(startPos, findIdx - startPos);
		out += newString;
		startPos = findIdx + templeteLength;
	}
	out += str.substr(startPos);
	return out;
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
