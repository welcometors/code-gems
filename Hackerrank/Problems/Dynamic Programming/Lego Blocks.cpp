// https://www.hackerrank.com/challenges/lego-blocks

#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
using namespace std;

#define natural unsigned long long
#define Limit 1000000007

vector<int> legoTypes = { 1, 2, 3, 4 };
unordered_map<int, natural> legoMap;

natural arrangements(int val){
	if (val < legoTypes[0])
		return 0;

	unordered_map<int, natural>::iterator itr = legoMap.find(val);

	if (itr != legoMap.end())
		return itr->second;

	natural ways = 0;
	for (auto c : legoTypes){
		if (c < val){
			ways = (ways + arrangements(val - c)) % Limit;
			continue;
		}
		if (c == val)
			ways = (ways + 1) % Limit;
		break;
	}

	legoMap.insert(unordered_map<int, natural>::value_type(val, ways));
	return ways;
}

natural IpowJmodLimit(natural i, natural j){
	if (i == 1)
		return 1;

	if (i == 0)
		return 0;

	natural p = 1;

	while (j){
		if (j & 1)
			p = (p * i) % Limit;
		i = (i * i) % Limit;
		j >>= 1;
	}

	return p;
}

int main(){

	int t;
	cin >> t;

	while (t--){
		int n, m;
		cin >> n >> m;

		vector<natural> solidWays(m + 1);
		solidWays[0] = 0;
		solidWays[1] = 1;

		vector<natural> totalWays(m + 1);
		totalWays[0] = 0;
		totalWays[1] = 1;

		for (int i = 2; i <= m; i++){
			natural nonSolidWays = 0;
			
			for (int j = 1; j < i; j++){
				nonSolidWays = (nonSolidWays + ((solidWays[j] * totalWays[i - j]) % Limit)) % Limit;
			}

			totalWays[i] = IpowJmodLimit(arrangements(i), n);
			solidWays[i] = (totalWays[i] + Limit - nonSolidWays) % Limit;
		}
		
		cout << solidWays[m] << endl;
	}

#ifdef _MSC_VER
	std::system("pause");
#endif

	return 0;
}

// 4 2 2 3 2 2 3 4 4