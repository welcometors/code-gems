#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
using namespace std;

unordered_map<string, unsigned int> facilityToIdMapping;
unsigned int getFacilityId(const string &facility){
	auto result = facilityToIdMapping.find(facility);
	if (result != facilityToIdMapping.end())
		return result->second;

	auto id = facilityToIdMapping.size();
	facilityToIdMapping[facility] = id;
	return id;
}

typedef struct{
	unsigned int price;
	unordered_set<unsigned int> facilityIds;
	unsigned int numberOfFacilities;
	unsigned int id;
}hotel;

struct eligibleHotel{
	unsigned int price;
	unsigned int numberOfFacilities;
	unsigned int id;

	eligibleHotel(unsigned int _id, unsigned int _price, unsigned int _numberOfFacilities) 
		: id(_id), price(_price), numberOfFacilities(_numberOfFacilities){}
};

vector<string> split(const string &s, char delimiter){
	vector<string> tokens;
	size_t start = 0, length = 0;

	for (auto c : s) {
		if (c == delimiter){
			if (length == 0){
				start++;
				continue;
			}
			tokens.push_back(s.substr(start, length));
			start += length + 1;
			length = 0;
		}
		else
			length++;
	}

	if (length != 0)
		tokens.push_back(s.substr(start, length));

	return tokens;
}

int main(){
	std::ios::sync_with_stdio(false);
	cin.tie(0);

	unsigned int N;
	cin >> N;
	cin.ignore();

	vector<hotel> hotels;
	for (unsigned int i = 0; i < N; i++){
		string line;
		getline(cin, line);
		auto values = split(line, ' ');

		hotel h;
		h.id = stoi(values[0]);
		h.price = stoi(values[1]);
		for (unsigned int i = 2, n = values.size(); i < n; i++)
			h.facilityIds.insert(getFacilityId(values[i]));
		h.numberOfFacilities = h.facilityIds.size();
		hotels.emplace_back(h);
	}

	unsigned int M;
	cin >> M;
	cin.ignore();

	for (unsigned int i = 0; i < M; i++){
		string line;
		getline(cin, line);
		auto values = split(line, ' ');

		auto budget = stoi(values[0]);
		vector<unsigned int> requiredFacilities;
		for (unsigned int i = 1, n = values.size(); i < n; i++)
			requiredFacilities.emplace_back(getFacilityId(values[i]));

		vector<eligibleHotel> eligibleHotels;
		for (auto &h : hotels){
			bool eligible = h.price <= budget;

			if (eligible){
				for (auto f : requiredFacilities){
					if (h.facilityIds.find(f) == h.facilityIds.end()){
						eligible = false;
						break;
					}
				}
			}

			if (eligible)
				eligibleHotels.emplace_back(h.id, h.price, h.numberOfFacilities);
		}

		std::sort(eligibleHotels.begin(), eligibleHotels.end(),
			[](const eligibleHotel &left, const eligibleHotel &right) {
				if (left.numberOfFacilities != right.numberOfFacilities)
					return left.numberOfFacilities > right.numberOfFacilities;
				if (left.price != right.price)
					return left.price < right.price;
				return left.id < right.id;
			}
		);

		for (auto& h : eligibleHotels){
			cout << h.id << " ";
		}
		cout << endl;
	}

	system("pause");
}

/*
4
1 70 wifi pool restaurant bathtub kitchenette
2 80 pool spa restaurant air-conditioning bathtub wifi
3 60 restaurant air-conditioning wifi
4 50 kitchenette
4
65 wifi
50 wifi
100 pool restaurant
80 kitchenette
*/