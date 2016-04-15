#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>
using namespace std;

class xmlelem {
	static const regex tagNameRegex;
	static const regex attribNameRegex;
	static const regex attribValuesRegex;
	static const string notFoundMessage;

public:
	string name;
	map<string,xmlelem> children;
	map<string, string> attributes;

	size_t parse(const vector<string> &input, size_t curidx) {
		auto tagname = sregex_token_iterator(input[curidx].begin(), input[curidx].end(), tagNameRegex, 1);
		auto iteratorend = sregex_token_iterator();
		name = tagname->str();
		
		auto attribnames = sregex_token_iterator(input[curidx].begin(), input[curidx].end(), attribNameRegex, 1);
		auto attribvalues = sregex_token_iterator(input[curidx].begin(), input[curidx].end(), attribValuesRegex, 1);
		for (auto it1 = attribnames, it2 = attribvalues; it1 != iteratorend; ++it1, ++it2)
			attributes[it1->str()] = it2->str();
		
		curidx++;
		while ((tagname = sregex_token_iterator(input[curidx].begin(), input[curidx].end(), tagNameRegex, 1)) != iteratorend){
			children[tagname->str()] = xmlelem();
			curidx = children[tagname->str()].parse(input, curidx);
		}
		return curidx + 1;
	}

	string get(const string& query) {
		size_t idx;
		if ((idx = query.find('.')) != string::npos) {
			string tagname = query.substr(0, idx);
			auto child = children.find(tagname);
			if(child != children.end())
				return child->second.get(query.substr(idx + 1));
			return notFoundMessage;
		}
		if ((idx = query.find('~')) != string::npos) {
			string tagname = query.substr(0, idx);
			auto child = children.find(tagname);
			if (child != children.end()) {
				string attribname = query.substr(idx + 1);
				auto attrib = child->second.attributes.find(attribname);
				if (attrib != child->second.attributes.end())
					return attrib->second;
			}
		}
		return notFoundMessage;
	}
};

const regex xmlelem::tagNameRegex = regex{ R"(<(\w+)(\s|>))" };
const regex xmlelem::attribNameRegex = regex{ R"(\s(\S+)\s\=)" };
const regex xmlelem::attribValuesRegex = regex{ R"(\"(\S+)\")" };
const string xmlelem::notFoundMessage = "Not Found!";

int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);

	int n, q;
	cin >> n >> q;
	cin.ignore();
	vector<string> xmldoc{"<root>"};
	for (int i = 0; i < n; i++){
		string s;
		getline(cin, s);
		xmldoc.emplace_back(s);
	}
	xmldoc.emplace_back("</root>");
	
	xmlelem root;
	root.parse(xmldoc, 0);

	for (int i = 0; i < q; i++) {
		string s;
		getline(cin, s);
		cout << root.get(s) << endl;
	}

#if _MSC_VER >= 1600
	system("pause");
#endif
}
