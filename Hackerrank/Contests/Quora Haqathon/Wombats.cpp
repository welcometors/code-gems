#include <iostream>
#include <vector>
using namespace std;

#define natural int

struct ball{
	natural value;
	ball* lft;
	ball* rit;
	ball* btm;

	ball(natural v) : value(v), lft(nullptr), rit(nullptr), btm(nullptr){}
};

struct layer{
	vector<ball*> balls;
};

int main(){
	
	int t;
	cin >> t;

	while (t--){
		int n, m;
		cin >> n >> m;
		
	}

	return 0;
}