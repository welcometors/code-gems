#include <vector>
#include <cstdlib>
#include <iostream>
using namespace std;

int main() {
	int n = 10000000;
	int freq[1000] = {};

	vector<int> ary(n);
	for (int i = 0; i < n; i++){
		ary[i] = 1 + (rand() % 999);
		freq[ary[i]]++;
	}

	cout << n << endl;
	for (int i = 1; i < 1000; i++)
		if (freq[i] && n > freq[i])
			n = n - freq[i];
			//cout << (n = n - freq[i]) << endl;

	return 0;
}