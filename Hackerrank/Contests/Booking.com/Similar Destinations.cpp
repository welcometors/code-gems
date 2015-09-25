#include <iostream>
#include <cstdint>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
using namespace std;

const unsigned char BitsCountTable256[256] = {
#define B2(n) n,     n+1,     n+1,     n+2
#define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
	B6(0), B6(1), B6(1), B6(2)
};

inline uint32_t bitCount32(uint32_t v){
	unsigned char *p = (unsigned char *)&v;
	return BitsCountTable256[p[0]] + BitsCountTable256[p[1]] +
		BitsCountTable256[p[2]] + BitsCountTable256[p[3]];
}

const size_t MAXTAGS = 1024;
const size_t MAXTAGBITS = MAXTAGS / 32;

class TagInt{
public:
	uint32_t values[MAXTAGBITS];

	TagInt(){
		memset(values, 0, MAXTAGS / 8);
	}

	inline void set(uint32_t pos){
		size_t idx = pos >> 5;
		size_t off = pos & 31;
		values[idx] |= 1 << off;
	}

	TagInt AND(const TagInt &t) const{
		TagInt r;
		for (int i = 0; i < MAXTAGBITS; i++)
			r.values[i] = values[i] & t.values[i];
		return r;
	}

	bool contains(const TagInt &t) const{
		for (int i = 0; i < MAXTAGBITS; i++)
			if ((values[i] & t.values[i]) != t.values[i])
				return false;
		return true;
	}

	uint32_t bitCount(){
		uint32_t count = 0;
		for (int i = 0; i < MAXTAGBITS; i++)
			count += bitCount32(values[i]);
		return count;
	}

	bool operator == (const TagInt &other) const{
		for (int i = 0; i < MAXTAGBITS; i++)
			if (values[i] != other.values[i])
				return false;
		return true;
	}
};

struct TagIntHasher
{
	std::size_t operator()(const TagInt& k) const{
		size_t h = 0;
		for (int i = 0; i < MAXTAGBITS; i++)
			h ^= hash<int>()(k.values[i]);
		return h;
	}
};

unordered_map<string, uint32_t> tagMap;
vector<string> tags;
uint32_t getTagId(const string &tag){
	auto result = tagMap.insert(unordered_map<string, uint32_t>::value_type(tag, tagMap.size()));
	if (result.second)
		tags.push_back(tag);
	return result.first->second;
}

uint32_t oldTagMapping[MAXTAGS];
void sortTags(){
	vector<pair<string, uint32_t>> oldTagOrder;
	for (int i = 0, n = tags.size(); i < n; i++)
		oldTagOrder.push_back(make_pair(tags[i], i));

	std::sort(oldTagOrder.begin(), oldTagOrder.end(),
		[](const pair<string, uint32_t> &left, const pair<string, uint32_t> &right) {
		return left.first < right.first;
	});

	tags.clear();
	for (int i = 0, n = oldTagOrder.size(); i < n; i++){
		tags.emplace_back(oldTagOrder[i].first);
		oldTagMapping[oldTagOrder[i].second] = i;
	}
}

string getTags(const TagInt &t){
	string result;
	for (int i = 0, j = 0; i < MAXTAGBITS; i++){
		for (int k = 0; k < 32; k++, j++){
			if ((t.values[i] >> k) & 1){
				result += tags[j];
				result += ',';
			}
		}
	}
	return result.substr(0, result.length() - 1);
}

class Location{
public:
	string name;
	unordered_set<uint32_t> tags;
	TagInt bitTags;

	void generateTagBits(){
		for (auto t : tags)
			bitTags.set(oldTagMapping[t]);
		tags.clear();
	}
};

int main(){
	std::ios::sync_with_stdio(false);
	cin.tie(0);

	uint32_t groupCountRequirement;
	cin >> groupCountRequirement;
	cin.ignore();

	vector<Location> locations;
	string line;
	while (getline(cin, line)){
		size_t start = 0;
		size_t end = line.find(':');
		Location l;
		l.name = line.substr(start, end - start);

		while (true){
			start = end + 1;
			end = line.find(',', start);
			if (end == string::npos)
				break;

			l.tags.insert(getTagId(line.substr(start, end - start)));
		}
		l.tags.insert(getTagId(line.substr(start)));

		locations.emplace_back(l);
	}
	tagMap.clear();

	sortTags();

	for (auto &l : locations){
		l.generateTagBits();
	}

	std::sort(locations.begin(), locations.end(),
		[](const Location &left, const Location &right) {
		return left.name < right.name;
	});

	int numOfLocations = locations.size();
	unordered_map<TagInt, uint32_t, TagIntHasher> groups;

	for (int i = 0; i < numOfLocations; i++){
		for (int j = i + 1; j < numOfLocations; j++){
			auto common = locations[i].bitTags.AND(locations[j].bitTags);
			auto count = common.bitCount();
			if (groupCountRequirement <= count)
				groups[common] = count;
		}
	}

	vector<tuple<string, string, uint32_t>> output;
	for (auto &g : groups){
		string left;
		for (auto &l : locations){
			if (l.bitTags.contains(g.first)){
				left += l.name;
				left += ',';
			}
		}
		output.emplace_back(left.substr(0, left.length() - 1), getTags(g.first), g.second);
	}

	std::sort(output.begin(), output.end(),
		[](const tuple<string, string, uint32_t> &left, const tuple<string, string, uint32_t> &right) {
		if (get<2>(left) != get<2>(right))
			return get<2>(left) > get<2>(right);
		return get<0>(left) < get<0>(right);
	});

	for (auto &o : output)
		cout << get<0>(o) << ":" << get<1>(o) << endl;

	system("pause");
}

/*
4
london:theatre,museums,monuments,food,parks,architecture,nightlife
amsterdam:museums,shopping,architecture,nightlife,cycling,food,walking
berlin:monuments,nightlife,food,architecture,city_trip
paris:shopping,food,monuments,architecture,gourmet,walking,museums
barcelona:architecture,shopping,beach,food,tapas,nightlife
*/