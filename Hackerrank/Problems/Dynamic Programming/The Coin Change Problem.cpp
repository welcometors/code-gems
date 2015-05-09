// https://www.hackerrank.com/challenges/coin-change

#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
using namespace std;

#define natural unsigned long
#define MAXCOINS 100

vector<int> coins;
unordered_map<int, natural> coinMap;

natural changes(int val, int startIdx = 0){
	if (startIdx >= coins.size() || val < coins[startIdx])
		return 0;

	unordered_map<int, natural>::iterator itr = coinMap.find(val * MAXCOINS + startIdx);

	if (itr != coinMap.end())
		return itr->second;

	natural ways = 0;
	for (int i = startIdx; i < coins.size(); i++){
		if (coins[i] < val){
			for (int j = val - coins[i]; j >=0; j -= coins[i]){
				ways += (j)? changes(j, i + 1): 1;
			}
			continue;
		}
		
		if (coins[i] == val)
			ways++;

		break;
	}

	coinMap.insert(unordered_map<int, natural>::value_type(val * MAXCOINS + startIdx, ways));
	return ways;
}

void split(const string &s, const string &delimiter, vector<string> &v){
	size_t start = 0, index = 0;

	while ((index = s.find(delimiter, start)) != std::string::npos) {
		string token = s.substr(start, index - start);
		if( !token.empty())
			v.push_back(token);
		start = index + delimiter.length();
	}

	string token = s.substr(start, s.length() - start);
	if (!token.empty())
		v.push_back(token);
}

int main(){
	
	{
		string coinsInput;
		vector<string> tokens;

		std::getline(cin, coinsInput);
		split(coinsInput, ", ", tokens);

		for (auto s : tokens)
			coins.push_back(std::stoi(s));

		std::sort(coins.begin(), coins.end());
	}

	int n;
	cin >> n;

	cout << changes(n) << endl;

#ifdef _MSC_VER
	std::system("pause");
#endif

	return 0;
}

// 1, 2, 3
// 4