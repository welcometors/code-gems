#include <iostream>
#include <ctype.h>
#include <string>
#include <algorithm>
#include <unordered_map>
using namespace std;

class TrieNode{
public:
	int matchCode;
	unordered_map<char, TrieNode*> nodes;

	TrieNode() : matchCode(0){}
};

class Trie{
public:
	TrieNode* root;

	Trie(){
		root = new TrieNode;
	}

	void freeNode(TrieNode* node){
		for (auto &v : node->nodes)
			freeNode(v.second);
		delete node;
	}

	~Trie(){
		freeNode(root);
	}

	void insert(int id, const string &str){
		TrieNode* cur = root;
		TrieNode* newNode = new TrieNode;

		for (auto c : str){
			auto result = cur->nodes.insert(unordered_map<char, TrieNode*>::value_type(tolower(c), newNode));
			if (result.second == true)
				newNode = new TrieNode;
			cur = result.first->second;
		}
		cur->matchCode = id;

		delete newNode;
	}

	void show(char c, TrieNode* node, int depth = 0){
		if (depth){
			for (int i = 0; i < depth - 1; i++)
				cout << "|  ";
			cout << "|- " << c;
			if (node->matchCode)
				cout << " : " << node->matchCode;
			cout << endl;
		}
		for (auto &v : node->nodes)
			show(v.first, v.second, depth+1);
	}

	void print(){
		show('*', root);
	}
};

int main(){
	std::ios::sync_with_stdio(false);
	cin.tie(0);

	unsigned int N;
	cin >> N;
	cin.ignore();

	vector<string> facilities = { " " };
	vector<bool> foundFacilities = { false };
	for (unsigned int i = 0; i < N; i++){
		string facility;
		getline(cin, facility);
		facilities.emplace_back(facility);
		foundFacilities.push_back(false);
	}
	std::sort(facilities.begin(), facilities.end());

	Trie facilitiesTrie;
	for (unsigned int i = 1; i <= N; i++){
		facilitiesTrie.insert(i, facilities[i]);
	}
	//facilitiesTrie.print();

	string text;
	getline(cin, text);

	auto& start = facilitiesTrie.root->nodes;
	for (int i = 0, n = text.length(); i < n; i++){
		auto result = start.find(tolower(text[i]));
		if (result != start.end()){
			TrieNode* cur = result->second;
			if (cur->matchCode)
				foundFacilities[cur->matchCode] = true;
			
			int j = i + 1;
			while (cur != nullptr){
				auto result = cur->nodes.find(tolower(text[j++]));
				if (result != cur->nodes.end()){
					cur = result->second;
					if (cur->matchCode)
						foundFacilities[cur->matchCode] = true;
				}
				else
					cur = nullptr;
			}
		}
	}

	for (int i = 1; i <= N; i++){
		if (foundFacilities[i])
			cout << facilities[i] << endl;
	}

	system("pause");
}

/*
5
wifi
swimming pool
garden
beach
tennis court
Our hotel is a very luxurious one, we are committed to provide the best holiday, you can always relax on our private beach, we provide four amazing tennis courts. WIFI is available in all areas.
*/