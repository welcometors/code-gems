// Author	: Rahul Sharma <rahulsrma26@gmail.com>

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int maxGcdPairSum(const vector<int>& a, const vector<int>& b) {
	int aMax = *max_element(a.begin(), a.end());
	vector<int> aFreq(aMax + 1, 0);
	for (auto x : a)
		aFreq[x]++;
	
	int bMax = *max_element(b.begin(), b.end());
	vector<int> bFreq(bMax + 1, 0);
	for (auto x : b)
		bFreq[x]++;

	for (int n = min(aMax, bMax); n; n--) {
		int gcdBA = (aMax / n)*n;
		for (; gcdBA; gcdBA -= n)
			if (aFreq[gcdBA])
				break;
		
		if (gcdBA) {
			int gcdAB = (bMax / n)*n;
			for (; gcdAB; gcdAB -= n)
				if (bFreq[gcdAB])
					break;

			if (gcdAB)
				return gcdBA + gcdAB;
		}
	}

	return 1;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	vector<int> ary1 = { 3, 1, 4, 2, 8 };
	vector<int> ary2 = { 5, 2, 12, 8, 3 };
	cout << maxGcdPairSum(ary1, ary2);
}