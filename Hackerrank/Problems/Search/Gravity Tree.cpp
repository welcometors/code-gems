#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//#pragma comment(linker, "/STACK:102400000,102400000")

struct quadratic {
	int a, b, c;

	int calculate(int d) {
		return a*d*d + b*d + c;
	}

	quadratic& operator+=(const quadratic& child) {
		a += child.a;
		b += 2 * child.a + child.b;
		c += child.a + child.b + child.c;
		return *this;
	}
};

struct node{
	int value, distance;
	node *parent;
	vector<node*> childs;
	// gravity = a*x*x + b*x + c
	quadratic gravity;
};

class tree {
	vector<node*> nodes;

	void inorder(node* parent) {
		if (parent != nullptr) {
			cout << "(" << parent->value << "," << parent->distance << ") ";
			for(auto child: parent->childs)
				inorder(child);
		}
	}

	void feedDistance(node* parent, int d) {
		parent->gravity = { 1,0,0 };
		parent->distance = d++;
		for (auto child : parent->childs) {
			feedDistance(child, d);
			parent->gravity += child->gravity;
		}
	}

	node* getLCA(node* u, node* v) {
		while (u->distance > v->distance)
			u = u->parent;
		while (u->distance < v->distance)
			v = v->parent;
		while(u != v)
			u = u->parent, v = v->parent;
		return u;
	}

public:
	tree(int n) {
		nodes = vector<node*>(n, nullptr);
		nodes[0] = new node{ 0, 0, nullptr };
	}

	void insert(int p, int c) {
		if (nodes[p] != nullptr) {
			if(nodes[c] == nullptr)
				nodes[c] = new node{ c, 0, nodes[p]};
			else
				nodes[c]->parent = nodes[p];
			nodes[p]->childs.push_back(nodes[c]);
		}
		else {
			nodes[p] = new node{ p, 0, nullptr};
			if (nodes[c] == nullptr)
				nodes[c] = new node{ c, 0, nodes[p] };
			else
				nodes[c]->parent = nodes[p];
			nodes[p]->childs.push_back(nodes[c]);
		}
	}

	void precalcDistanceFromRoot() {
		feedDistance(nodes[0], 0);
	}

	void displayInorder() {
		inorder(nodes[0]);
		cout << endl;
	}

	int getGravity(int u, int v) {
		if (u == v)
			return nodes[u]->gravity.calculate(0);

		node* lca = getLCA(nodes[u], nodes[v]);
		int relativeDistance = nodes[u]->distance + nodes[v]->distance - 2 * lca->distance;
		if (nodes[v] != lca)
			return nodes[v]->gravity.calculate(relativeDistance);
	
		int sum = nodes[u]->gravity.calculate(0), d = 1;
		node* cur = nodes[u];
		for (int i = 0, iter = nodes[u]->distance - nodes[v]->distance; i < iter; i++, d++, cur = cur->parent)
			sum += cur->parent->gravity.calculate(d) - cur->gravity.calculate(d+1);
		return sum;
	}
};

#define sort(x) sort(x.begin(), x.end())
#define input(type, var) type var; cin >> var
int main() {
	std::ios::sync_with_stdio(false);
	cin.tie(0);

	input(int, n);
	tree tree(n);
	for (int i = 1, c; i < n; i++) {
		cin >> c;
		tree.insert(c - 1, i);
	}

	tree.precalcDistanceFromRoot();
	tree.displayInorder();

	input(int, q);
	for (int i = 0, u, v; i<q; i++) {
		cin >> u >> v;
		cout << tree.getGravity(u - 1, v - 1) << endl;
	}
}

/*
5
1 2 2 4
2
2 1
1 4
============
5
4 2 5 1
3
5 5
2 5
5 2
----
14
6
13
*/