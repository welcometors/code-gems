// subset lexographically

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

#define natural unsigned int

string& getSubset(const string &str, string &out, int v, int n){
	int lw = 1 << n, s = 0, w;

	for (int i = 0; (w = lw) > 1 && i < n; i++, lw = w >> 1){
		while (w > 1 && s + w <= v){
			s += w;
			w >>= 1;
		}
		if (w > 1)
			v = s + ((v + w - 1) % w);
	}

	out.clear();
	for (int i = n - 1; i >= 0; i--, v >>= 1)
		if (!(v & 1))
			out = str[i] + out;
	return out;
}

int main(){
	string s, o;
	cin >> s;

	std::sort(s.begin(), s.end());
	//cout << s << endl;

	int l = s.length(), ns = 1 << s.length();

	for (int i = 1; i < ns; i++)
		cout << getSubset(s, o, i, l) << endl;

#ifdef _MSC_VER
	std::system("pause");
#endif
}
