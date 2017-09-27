#ifndef __BELLMAN_JACKS_CAR_RENTAL_H__
#define __BELLMAN_JACKS_CAR_RENTAL_H__

#include <iostream>
#include <cmath>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <algorithm>

namespace bellman {
	namespace jacksCarRental {
		using namespace std;

		template <class T>
		using matrixRow = vector<T>;

		template <class T>
		using matrix = vector<matrixRow<T>>;

		class Station {
		public:
			int rent, avgRequest, avgReturn, maxCars, maxCapacity;
			matrix<double> probabilities;
			vector<double> rewards;

			Station();
			Station(const Station&);
			Station(Station&&);
			Station& operator=(const Station&);
			Station& operator=(Station&&);

			Station(
				int rent, int maxCars, int maxCapacity, 
				int avgRequest, int avgReturn);
		};

		class TwoStationCompany {
			int maxCars, maxTransfer;
			double gamma, transferCost;
			Station a, b;
			matrix<double> V;
			matrix<int> policy;

			auto bellmanValue(int aCars, int bCars, int nTransfer);

		public:
			TwoStationCompany();
			TwoStationCompany(const TwoStationCompany&);
			TwoStationCompany(TwoStationCompany&&);
			TwoStationCompany& operator=(const TwoStationCompany&);
			TwoStationCompany& operator=(TwoStationCompany&&);

			TwoStationCompany(
				int maxCars, int maxTransfer,
				int rent, double transferCost,
				int avgRequestA, int avgReturnA,
				int avgRequestB, int avgReturnB, 
				double gamma);

			int evaluatePolicy(int iterationLimit = 100);

			int policyImprovement();

			matrix<double>& getValueMap();

			matrix<int>& getPolicy();
		};

	}
}

#endif // !__BELLMAN_JACKS_CAR_RENTAL_H__
