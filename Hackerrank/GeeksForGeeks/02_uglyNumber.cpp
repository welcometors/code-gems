// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/ugly-numbers/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int uglyNumber(size_t n) {
	int m2 = 2, m3 = 3, m5 = 5;
	int i2 = 0, i3 = 0, i5 = 0;
	vector<int> numbers = { 1 };

	for (size_t i = 1; i < n; i++) {
		int next = min({ m2, m3, m5 });
		numbers.push_back(next);
		if (next == m2)
			m2 = numbers[++i2] * 2;
		if (next == m3)
			m3 = numbers[++i3] * 3;
		if (next == m5)
			m5 = numbers[++i5] * 5;
	}

	return numbers.back();
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	cout << uglyNumber(150) << endl;
}