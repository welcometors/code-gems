// https://projecteuler.net/problem=85
/*
Counting rectangles
By counting carefully it can be seen that a rectangular grid measuring 3 by 2 contains eighteen rectangles:

Although there exists no rectangular grid that contains exactly two million rectangles, 
find the area of the grid with the nearest solution.

Solution:
Code complexity is O(n^1/4).
Idea is to solve the quadratic equation by assuming the width.
Number of rectangles in a wxh grid is w(w+1)/2.h(h+1)/2 = n
Let w(w+1)/2=p
Now, h^2+h-2n/p=0
Since, h can't be negative h=(sqrt(1+8n/p)-1)/2
We can go through every w. Assuming w is the smaller side, w<=(4n)^1/4.
Check own post: https://projecteuler.net/thread=85;page=8#254620
*/

#include <iostream>
#include <cmath>
#include <chrono>
using namespace std;
typedef unsigned long long natural;
typedef unsigned long long ull;
typedef long long ll;

ll compute() {
	ll best = 0, w = 0, h = 0, n = 2000000, sqrtsqrtK = sqrt(sqrt(4 * n)) + 1;
	for (ll x = 2; x <= sqrtsqrtK; x++) {
		ll p = (x + 1)*x / 2;
		ll y = (sqrt(1 + 8.0 * n / p) - 1) / 2;
		ll rectangles = ((x + 1)*x / 2)*((y + 1)*y / 2);
		if (abs(n - rectangles) < abs(n - best))
			best = rectangles, w = x, h = y;
	}
	cout << w << "x" << h << " has " << best << " rectangles." << endl;
	return w*h;
}

int main() {
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	cout << "Done in "
		<< duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1000000.0
		<< " miliseconds." << endl;
	cout << result << endl;
}