#include <iostream>
#include <unordered_map>
using namespace std;

#define natural unsigned long long
#define Limit 1000000007ULL

#define tableSize 10000
#define MAXN 1000
#define DIGITS 10
natural *nCrTable;
natural *digitPow;

void compute(){
	int size = (tableSize + 2) / 2;
	size *= (size + 1);
	nCrTable = new natural[size];
	nCrTable[0] = nCrTable[1] = 1;

	natural *ptr = &nCrTable[2], *lptr = &nCrTable[1];

	for (int i = 2; i <= tableSize; i++){
		*ptr++ = 1;

		for (int j = 1, l = ((i + 2) >> 1) - 1 + (i & 1); j < l; j++, lptr++)
			*ptr++ = (*lptr + *(lptr + 1)) % Limit;

		if (!(i & 1))
			*ptr++ = (*lptr + *lptr) % Limit;

		lptr++;
	}

	digitPow = new natural[MAXN * DIGITS];
	for (int i = 1, l = 0; i <= DIGITS; i++){
		digitPow[l++] = 1;
		for (int j = 1; j < MAXN; j++, l++){
			digitPow[l] = (digitPow[l - 1] * i) % Limit;
		}
	}
}

natural nCr(int n, int r){
	if (r > n)
		return 0;
	else if (r > n - r)
		r = n - r;

	if (n < 2 || r == 0)
		return 1;

	int k = (n + 2) >> 1;

	return nCrTable[(k - 1) * k + (n & 1) * k + r];
}

int numbers(int l, int r, int k){
	int t = 0;
	for (int n = l; n <= r; n++){
		int dc[10] = {}, c = 0;
		for (int i = n; i; i /= 10)
			dc[i % 10]++;
		for (int i = 0; i < 10; i++)
			c += dc[i] <= k;
		if (c == 10)
			t++;
	}
	return t;
}

unordered_map<int, natural> nFkMap;
int nFk(int n, int k, int c){
	if (n <= k)
		return digitPow[(c - 1) * MAXN + n];

	int idx = (c * MAXN + n)* MAXN + k;
	unordered_map<int, natural>::iterator it = nFkMap.find(idx);
	if (it != nFkMap.end())
		return it->second;

	natural r = (c * ((nFk(n - 1, k, c) + Limit - ((nCr(n - 1, k) * nFk(n - 1 - k, k, c - 1)) % Limit)) % Limit)) % Limit;
	nFkMap.insert(unordered_map<int, natural>::value_type(idx, r));
	return r;
}

int ways(int n, int k){
	return (9 * ((nFk(n - 1, k, 10) + Limit - ((nCr(n - 1, k) * nFk(n - 1 - k, k, 9)) % Limit)) % Limit)) % Limit;
}

int main(){
	compute();

	int t, n, k;
	cin >> t;

	while (t--){
		cin >> n >> k;

		natural x = 9;
		if (n <= k)
			x = (x * digitPow[9 * MAXN + n - 1]) % Limit;
		else if (10 * k < n)
			x = 0;
		else
			x = ways(n, k) % Limit;

		cout << x << endl;
	}

	delete[] digitPow;
	delete[] nCrTable;
	return 0;
}
