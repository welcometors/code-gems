// https://www.hackerrank.com/contests/quora-haqathon/challenges/relatedquestions

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <algorithm>
using namespace std;

#define natural uint32_t
#define real double

typedef unordered_map<int, real> timeEdge;

struct node{
	natural time;
	timeEdge links;

	node(natural t) : time(t){}
};

typedef vector<node> graph;

int numQues;
graph inputGraph;

real getAvgTime(int vertex, int ancestor){
	auto &sourceLinks = inputGraph[vertex].links;
	timeEdge::iterator itr = sourceLinks.find(ancestor);

	if (itr->second != -1)
		return itr->second;

	real avgTime = 0;
	for (auto &n : sourceLinks){
		if (n.first != ancestor){
			avgTime += getAvgTime(n.first, vertex);
		}
	}

	avgTime /= (sourceLinks.size() - 1);
	avgTime += inputGraph[vertex].time;

	itr->second = avgTime;
	return avgTime;
}

int getOptimal(){
	
	for (auto &v : inputGraph){
		if (v.links.size() == 1){
			v.links.begin()->second = v.time;
		}
	}

	real minTime = numeric_limits<real>::max();
	int minIdx = 0;

	for (int i = 0; i < numQues; i++){
		auto &rootLinks = inputGraph[i].links;
		
		if (rootLinks.size() == 1)
			continue;
		
		real time = 0.0;

		for (auto &v : rootLinks){
			time += getAvgTime(v.first, i);
		}

		time /= rootLinks.size();
		time += inputGraph[i].time;

		if (time < minTime){
			minTime = time;
			minIdx = i;
		}
	}

	return minIdx;
}

void breakRandom(int root, int l, int r){
	if (l > r)
		return;

	if (l == r){
		inputGraph[root].links.insert(timeEdge::value_type(l, -1));
		inputGraph[l].links.insert(timeEdge::value_type(root, -1));
		//cout << root << " -> " << l << endl;
		return;
	}

	int child = l + (rand() % (r - l + 1));
	inputGraph[root].links.insert(timeEdge::value_type(child, -1));
	inputGraph[child].links.insert(timeEdge::value_type(root, -1));
	//cout << root << " -> " << child<< endl;
	breakRandom(child, l, child - 1);
	breakRandom(child, child + 1, r);
}

void generateRandom(int size, int limit){
	numQues = size;
	inputGraph.reserve(numQues);

	for (int i = 0; i < numQues; i++)
		inputGraph.push_back(node(rand() % limit));

	int root = 1 + rand() % (size - 2);
	//cout << root << endl;
	breakRandom(root, 0, root - 1);
	breakRandom(root, root + 1, numQues - 1);
}

int main(){
	cin >> numQues;

	if (numQues < 3){
		for (int i = 0, x; i < numQues; i++)
			cin >> x;
		
		for (int i = 1, u, v; i < numQues; i++)
			cin >> u >> v;

		cout << "1" << endl;
		return 0;
	}

	stringstream sin;
	sin << cin.rdbuf();

	for (int i = 0, x; i < numQues; i++){
		sin >> x;
		inputGraph.push_back(node(x));
	}

	for (int i = 1, u, v; i < numQues; i++){
		sin >> u >> v;
		inputGraph[u - 1].links.insert(timeEdge::value_type(v - 1, -1));
		inputGraph[v - 1].links.insert(timeEdge::value_type(u - 1, -1));
	}

	cout << getOptimal() + 1 << endl;
	return 0;
}

// 5  2 2 1 2 2  1 2  2 3  3 4  4 5
