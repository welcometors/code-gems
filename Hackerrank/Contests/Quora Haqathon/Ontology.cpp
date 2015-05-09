// https://www.hackerrank.com/contests/quora-haqathon/challenges/ontology

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <limits>
#include <algorithm>
using namespace std;

#define natural uint32_t
#define real double

struct topic{
	string title;
	vector<topic*> subTopics;
	vector<string> questions;

	topic(string name) : title(name){}
};

typedef unordered_map<string, topic*> topicMap;
typedef topicMap::value_type topicMapPair;

class TopicTree{
	topic *root;
	topicMap topicHash;

	void readChilds(topic* parent, stringstream &ss){
		topic* child = nullptr;

		do {
			string input;
			ss >> input;

			if (input.front() == ')')
				return;
			else if (input.front() == '('){
				readChilds(child, ss);
				continue;
			}

			child = new topic(input);
			topicHash.insert(topicMapPair(input, child));
			//cout << input << " < " << parent->title << endl;
			parent->subTopics.push_back(child);
		} while (true);
	}

	unordered_map<string, uint32_t> findHistory;

	uint32_t findQueries(topic* parent, const string &query){
		unordered_map<string, uint32_t>::iterator historyItr = findHistory.find(parent->title + query);
		if (historyItr != findHistory.end())
			return historyItr->second;

		uint32_t queryCount = 0;

		for (auto q : parent->questions){
			if (!q.compare(0, query.length(), query))
				queryCount++;
		}

		for (auto c : parent->subTopics){
			queryCount += findQueries(c, query);
		}

		findHistory.insert(unordered_map<string, uint32_t>::value_type(parent->title + query, queryCount));
		return queryCount;
	}

public:
	TopicTree(stringstream &ss){
		int n;
		ss >> n;
		
		if (n == 0){
			root = nullptr;
			return;
		}

		string input;
		ss >> input;
		//cout << "root = " << input << endl;

		root = new topic(input);
		topicHash.insert(topicMapPair(input, root));

		if (--n){
			ss >> input;
			if (input.front() == '(')
				readChilds(root, ss);
		}
	}

	void insertQuestion(string &question){
		int colonIndex = question.find(':');
		topicMap::iterator topicItr = topicHash.find(question.substr(0, colonIndex));
		
		if (topicItr != topicHash.end()){
			//cout << topicItr->second->title << " [" << question.substr(colonIndex+2) << "]" << endl;
			topicItr->second->questions.push_back(question.substr(colonIndex + 2));
		}
	}

	uint32_t find(const string &query){
		int spaceIndex = query.find(' ');
		topicMap::iterator topicItr = topicHash.find(query.substr(0, spaceIndex));

		if (topicItr != topicHash.end()){
			string prefix = query.substr(spaceIndex + 1);
			//cout << topicItr->second->title << " [" << prefix << "]" << endl;
			return findQueries(topicItr->second, prefix);
		}

		return 0;
	}
	
};

int main(){
	stringstream ssin;
	ssin << cin.rdbuf();

	TopicTree topics(ssin);
	
	int questions;
	ssin >> questions;
	ssin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	while (questions--){
		string ques;
		getline(ssin, ques);
		topics.insertQuestion(ques);
	}

	int quries;
	ssin >> quries;
	ssin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	while (quries--){
		string query;
		getline(ssin, query);
		cout << topics.find(query) << endl;
	}

	return 0;
}
