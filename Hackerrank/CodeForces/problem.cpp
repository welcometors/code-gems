#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef unsigned long long natural;

#define sort(x) sort(x.begin(), x.end())
#define input(type, var) type var; cin >> var
#define Modulus 10007

struct node {
	int color;
	int number;
	node *parent;
	node *childs[2];
};

struct input {
	int color;
	int unused = 1;
	int neighbours[3] = {};

	void addNeighbour(int v) {
		for(int i=0; i<3; i++)
			if (!neighbours[i]) {
				neighbours[i] = v;
				return;
			}
	}
};

void addChilds(node* root, vector<input>& boars) {
	for(int i=0, n, c=0; i<3; i++)
		if ((n = boars[root->number - 1].neighbours[i]) && boars[n - 1].unused) {
			root->childs[c] = new node{ boars[n - 1].color, n, root, {nullptr , nullptr} };
			boars[n - 1].unused = 0;
			addChilds(root->childs[c], boars);
			c++;
		}
}

node* getTree(vector<input>& boars) {
	node* root = new node{ boars[0].color, 1, nullptr, {nullptr, nullptr} };
	boars[0].unused = 0;
	addChilds(root, boars);
	return root;
}

void printTree(node *root) {
	if (root != nullptr) {
		cout << root->number << " -> ";
		root->color *= -1;
		printTree(root->childs[0]);
		printTree(root->childs[1]);
	}
}

int main() {
	std::ios::sync_with_stdio(false);
	cin.tie(0);

	input(int, n);
	vector<input> boars(n);
	for (int i = 0; i < n; i++)
		cin >> boars[i].color;
	for (int i = 1, x, y; i < n; i++) {
		cin >> x >> y;
		boars[x - 1].addNeighbour(y);
		boars[y - 1].addNeighbour(x);
	}

	auto root = getTree(boars);
	printTree(root);

#if _MSC_VER >= 1600
	system("pause");
#endif
}