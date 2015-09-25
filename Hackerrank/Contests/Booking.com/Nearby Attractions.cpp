#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
using namespace std;

typedef double real;

typedef struct{
	real lat;
	real lon;
}point;

typedef struct{
	point location;
	int id;
}attraction;

const double EARTH_RADIUS = 6371; //in km

real inline degree2radians(double degree){
	return 3.14159265359*degree/180.0;
}

real sphericalDistance(const point &p1, const point &p2) {
	auto p1_lat_in_radians = degree2radians(p1.lat);
	auto p2_lat_in_radians = degree2radians(p2.lat);
	auto p1_long_in_radians = degree2radians(p1.lon);
	auto p2_long_in_radians = degree2radians(p2.lon);

	return acos(sin(p1_lat_in_radians) * sin(p2_lat_in_radians) +
		cos(p1_lat_in_radians) * cos(p2_lat_in_radians) *
		cos(p2_long_in_radians - p1_long_in_radians)) * EARTH_RADIUS;
}

inline double roundedDistance(double d){
	return round(d * 100.0) / 100.0;
}

inline double getSpeed(const string &mode){
	if (mode[0] == 'm')
		return 20.0;
	if (mode[0] == 'b')
		return 15.0;
	return 5.0;
}

int main(){
	std::ios::sync_with_stdio(false);
	cin.tie(0);

	int N;
	cin >> N;
	
	vector<attraction> attractions;
	for (int i = 0; i < N; i++){
		attraction a;
		cin >> a.id >> a.location.lat >> a.location.lon;
		attractions.emplace_back(a);
	}

	int M;
	cin >> M;
	for (int i = 0; i < M; i++){
		point hotel;
		string mode;
		double time; // in minutes
		cin >> hotel.lat >> hotel.lon >> mode >> time;

		auto speed = getSpeed(mode);
		auto maxDistance = time * speed / 60.0;

		vector<pair<int, double>> reachableAttractions;
		for (auto &a : attractions){
			auto distance = roundedDistance(sphericalDistance(hotel, a.location));
			if (distance <= maxDistance)
				reachableAttractions.emplace_back(a.id, distance);
		}

		std::sort(reachableAttractions.begin(), reachableAttractions.end(), 
			[](const pair<int, double> &left, const pair<int, double> &right) {
				if (left.second != right.second)
					return left.second < right.second;
				return left.first < right.first;
			}
		);

		for (auto& p : reachableAttractions){
			cout << p.first << " ";
		}
		cout << endl;
	}

	system("pause");
}

/*
10
1 52.378281 4.900070
2 52.373634 4.890289
3 52.375737 4.896547
4 52.372995 4.893096
5 52.376237 4.902860
6 52.367066 4.893381
7 52.366537 4.911348
14 52.368832 4.892744
15 52.357895 4.892835
35 52.342497 4.855094
5
52.379141 4.880590 metro 80
52.358835 4.893867 foot 60
52.375859 4.886006 foot 30
52.371700 4.899070 metro 30
52.364055 4.898446 foot 60
*/