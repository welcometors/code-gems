#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define natural unsigned long long
#define Limit 1000000007ULL

natural IpowJ(natural i, natural j){
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
	int n, k, x;
	cin >> n >> k;

	vector<int> b(n);
	for (int i = 0; i < n; i++)
		cin >> b[i];
	
	std::sort(b.begin(), b.end());

	if (n == k)
		cout << b[b.size() - 1] - b[0] << endl;
	else{
		natural min = 0, max = 0, iCk_1 = 1;
		for (int i = k - 1; i < n; i++){
			min = (min + ((iCk_1 * b[n - 1 - i]) % Limit)) % Limit;
			max = (max + ((iCk_1 * b[i]) % Limit)) % Limit;
			iCk_1 = (iCk_1 * (i + 1)) % Limit;
			iCk_1 = (iCk_1 * IpowJ(i + 2 - k, Limit - 2)) % Limit;
		}
		natural diff = (Limit + max - min) % Limit;
		cout << diff << endl;
	}
}

/*
suppose ball numbers S = {a, b, c, d, e} are in increasing order
and k = 3
Let f(X,Y) is the number of sets containing X but not Y

* f({a},{})    = 4C2 {a, not a, not a}
* f({b},{a})   = 3C2 {b, not a or b, not a or b}
* f({c},{a,b}) = 2C2 {c, not a or b or c, not a or b or c}

so a will be minimum in 4C2 sets, similarily b and c will be minimum 
in 3C2 and 2C2 sets. So, sum of all minimums in the set of 3 is 
 = a*4C2+b*3C2+c*2C2. 
Similar thing can be done for maximum. Answer will be 
(sum of maximums) - (sum of minimums).
*/