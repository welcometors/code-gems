#include <iostream>
#include <unordered_map>
using namespace std;

#define natural unsigned int
#define MAXSIZE 1000
#define CHANGES 4

int worth[CHANGES] = { 1, 2, 5, 10 };
int coins[CHANGES] = {};
int limit[CHANGES] = {};

unordered_map<int, natural> ways;
natural findWays(int price, int level){
	if (level == 0)
		return (price <= limit[0]) ? 1 : 0;

	unordered_map<int, natural>::iterator it = ways.find(price*CHANGES + level);
	if (it != ways.end())
		return it->second;
	
	natural s = (price % worth[level] == 0 && price <= worth[level]*coins[level])? 1: 0;
	for (int i = price, j = 0; i > 0 && j <= coins[level]; i -= worth[level], j++)
		if (i <= limit[level - 1])
			s += findWays(i, level -1);

	ways.insert(unordered_map<int, natural>::value_type(price*CHANGES + level, s));
	return s;
}

int main(){
	int t, n;
	cin >> t;

	while (t--){
		cin >> n;
		for (int i = 0; i < CHANGES; i++)
			cin >> coins[i];
		
		limit[0] = coins[0] * worth[0];
		for (int i = 1; i < CHANGES; i++)
			limit[i] = limit[i - 1] + coins[i] * worth[i];

		cout << findWays(n, 3) << endl;
		ways.clear();
	}
}