#include <iostream>
#include <limits>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <unordered_map>
using namespace std;

typedef double real;
typedef pair<int, real> hotel;
typedef vector<hotel> city;

#define MAXCITIES 10

inline real round2(real d){
	return round(d * 100.0) / 100.0;
}

vector<city> cities;
unordered_map<size_t, real> budgetMap;

real getBestScore(size_t city, size_t budget){
	auto idx = budget*MAXCITIES + city;
	auto result = budgetMap.find(idx);
	if (result != budgetMap.end())
		return result->second;

	real maxscore = -numeric_limits<real>::max();
	for (auto &h : cities[city]){
		if (budget < h.first)
			break;

		auto score = h.second;
		if (city)
			score += getBestScore(city - 1, budget - h.first);
		if (score > maxscore)
			maxscore = score;
	}
	budgetMap[idx] = maxscore;
	return maxscore;
}

int main(){
	std::ios::sync_with_stdio(false);
	cin.tie(0);

	int minReqBudget = 0;
	size_t numCities, userBudget;
	cin >> numCities >> userBudget;

	for (int i = 0, n; i < numCities; i++){
		cin >> n;
		unordered_map<int, real> hotelStats;
		for (int j = 0; j < n; j++){
			int price; real score;
			cin >> price >> score;
			if (price <= userBudget){
				if (hotelStats.find(price) != hotelStats.end()){
					if (score > hotelStats[price])
						hotelStats[price] = score;
				}
				else
					hotelStats[price] = score;
			}
		}

		if (hotelStats.size() < 1){
			minReqBudget = userBudget + 1;
			break;
		}

		city newCity;
		for (auto &p : hotelStats){
			newCity.emplace_back(p.first, p.second);
		}

		std::sort(newCity.begin(), newCity.end(),
			[](const hotel &left, const hotel &right) {
			return left.first < right.first;
		});

		city filteredCity = { newCity.front() };
		real currentScore = filteredCity.front().second;
		for (auto &h : newCity){
			if (h.second > currentScore){
				currentScore = h.second;
				filteredCity.push_back(h);
			}
		}

		minReqBudget += filteredCity.front().first;
		cities.emplace_back(filteredCity);
	}

	if (userBudget < minReqBudget)
		cout << "-1" << endl;
	else
		cout << fixed << setprecision(2) << round2(getBestScore(numCities - 1, userBudget)) << endl;

	system("pause");
}