// template

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main(){
	stringstream ssin, ssout;
	ssin << cin.rdbuf();
	


	cout << ssout.rdbuf();
	system("pause");
	return 0;
}

// test