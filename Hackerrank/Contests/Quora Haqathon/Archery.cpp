// https://www.hackerrank.com/contests/quora-haqathon/challenges/archery

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

#define real double
#define distance(x, y) sqrt((real)(x)*x + (real)(y)*y)

int findSmallerRadius(real radius, const vector<real> &radii){
	int l = 0, r = radii.size() - 1;

	while (l <= r){
		int m = (l + r) >> 1;

		if (radius < radii[m])
			r = m - 1;
		else
			l = m + 1;
	}

	return l;
}

int main(){
	int n, m, intersections = 0;
	cin >> n;

	vector<real> radii;

	for (int i = 0, x; i < n; i++){
		cin >> x;
		radii.push_back(x);
	}

	std::sort(radii.begin(), radii.end());
    
	cin >> m;

	for (int i = 0; i < m; i++){
		int x, y, r1, r2;
		
		cin >> x >> y;
		r1 = findSmallerRadius(distance(x,y), radii);
		
		cin >> x >> y;
		r2 = findSmallerRadius(distance(x,y), radii);

		intersections += (r1 > r2)? (r1 - r2): (r2 - r1);
	}

	cout << intersections << endl;
	return 0;
}
