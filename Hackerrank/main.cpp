#include <iostream>
#include <vector>
#include <algorithm>

#define sort(x) sort(x.begin(), x.end())
#define input(type, var) type var; cin >> var

int main() {
	using namespace std;
	ios::sync_with_stdio(false);
	cin.tie(0);

	input(int, t);
	while (t--) {
		input(int, n);
	}

#if _MSC_VER >= 1600
	system("pause");
#endif
}