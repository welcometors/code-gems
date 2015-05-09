// https://www.hackerrank.com/challenges/the-quickest-way-up

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

vector<string> split(const string &s, const string &delimiter){
	vector<string> tokens;
	size_t start = 0, index = 0;

	while ((index = s.find(delimiter, start)) != std::string::npos) {
		string token = s.substr(start, index - start);
		if (!token.empty())
			tokens.push_back(token);
		start = index + delimiter.length();
	}

	string token = s.substr(start, s.length() - start);
	if (!token.empty())
		tokens.push_back(token);

	return tokens;
}

struct edge{
	edge(int u, int v, int c) : u(u), v(v), c(c){}
	
	int u, v;
	int c;
};

int main(){
	int t;
	cin >> t;

	while (t--){
		int numLadders, numSnakes;
		cin >> numLadders >> numSnakes;

		vector<int> significantSquares = { 1, 100 };
		vector<pair<int, int>> ladders;
		vector<pair<int, int>> snakes;
		
		for (int i = 0, start, end; i < numLadders; i++){
			cin >> start >> end;
			ladders.push_back(make_pair(start, end));
			significantSquares.push_back(start);
			significantSquares.push_back(end);
		}

		for (int i = 0, head, tail; i < numSnakes; i++){
			cin >> head >> tail;
			snakes.push_back(make_pair(head, tail));
			significantSquares.push_back(head);
			significantSquares.push_back(tail);
		}

		std::sort(significantSquares.begin(), significantSquares.end());
		vector<edge> edgeList;

		for( int i=0; i<significantSquares.size(); i++){
			for (int j = i + 1; j < significantSquares.size(); j++){

			}
		}
	}

	system("pause");
	return 0;
}

// 5 1 3 1 4 4 4 25 11 25 12