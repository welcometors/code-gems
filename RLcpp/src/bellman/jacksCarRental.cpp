#include "bellman\jacksCarRental.hpp"

namespace bellman::jacksCarRental {

	auto poisson(double lambda, int n) {
		vector<double> probabilities(n + 1, exp(-lambda));
		for (int k = 1; k <= n; k++)
			probabilities[k] = (probabilities[k - 1] * lambda) / k;
		return probabilities;
	}

	Station::Station() {}
	Station::Station(const Station&) = default;
	Station::Station(Station&&) = default;
	Station& Station::operator=(const Station&) = default;
	Station& Station::operator=(Station&&) = default;
		
	Station::Station(int _rent, int _maxCars, int _maxCapacity, int _avgRequest, int _avgReturn) :
		rent(_rent), maxCars(_maxCars), maxCapacity(_maxCapacity), 
		avgRequest(_avgRequest), avgReturn(_avgReturn),
		probabilities(_maxCapacity + 1, matrixRow<double>(_maxCapacity + 1, 0.0)),
		rewards(_maxCapacity + 1, 0.0){

		auto requestPDF = poisson(avgRequest, 10 * avgRequest);
		auto returnPDF = poisson(avgReturn, 10 * avgReturn);

#ifdef jacksRentalCar_Reward_bug
		// old bugged code (not depending on car returns)
		for (int cars = 0; cars <= maxCapacity; ++cars)
			for (int requests = 0; requests < requestPDF.size(); ++requests)
				rewards[cars] += 10.0 * min(cars, requests) * requestPDF[requests];
#else	
		for (int cars = 0; cars <= maxCapacity; ++cars)
			for (int requests = 0; requests < requestPDF.size(); ++requests)
				for (int returns = 0; returns < returnPDF.size(); ++returns)
					rewards[cars] += rent * min(cars + returns, requests)
						* requestPDF[requests] * returnPDF[returns];
#endif

		for (int requests = 0; requests < requestPDF.size(); ++requests)
			for (int returns = 0; returns < returnPDF.size(); ++returns)
				for (int cars = 0; cars <= maxCapacity; ++cars)
					probabilities[cars][min(cars + returns - min(cars, requests), maxCars)]
					+= requestPDF[requests] * returnPDF[returns];
	}

	TwoStationCompany::TwoStationCompany() {}
	TwoStationCompany::TwoStationCompany(const TwoStationCompany&) = default;
	TwoStationCompany::TwoStationCompany(TwoStationCompany&&) = default;
	TwoStationCompany& TwoStationCompany::operator=(const TwoStationCompany&) = default;
	TwoStationCompany& TwoStationCompany::operator=(TwoStationCompany&&) = default;
		
	TwoStationCompany::TwoStationCompany(int _maxCars, int _maxTransfer,
			int rent, double _transferCost,
			int avgRequestA, int avgReturnA, 
			int avgRequestB, int avgReturnB, double _gamma):

			maxCars(_maxCars), maxTransfer(_maxTransfer),
			transferCost(_transferCost), gamma(_gamma), 
			a(rent, maxCars, maxCars + maxTransfer, avgRequestA, avgReturnA),
			b(rent, maxCars, maxCars + maxTransfer, avgRequestB, avgReturnB),
			V(_maxCars + 1, matrixRow<double>(_maxCars + 1, 0.0)),
			policy(_maxCars + 1, matrixRow<int>(_maxCars + 1, 0)){}

	auto TwoStationCompany::bellmanValue(int aCars, int bCars, int nTransfer) {
			nTransfer = max(-bCars, min(nTransfer, aCars));
			nTransfer = max(-maxTransfer, min(+maxTransfer, nTransfer));
			aCars -= nTransfer;
			bCars += nTransfer;
			double value = -transferCost * abs(nTransfer);
			for (int aNext = 0; aNext <= maxCars; ++aNext)
				for (int bNext = 0; bNext <= maxCars; ++bNext)
					value += a.probabilities[aCars][aNext] * b.probabilities[bCars][bNext] * (
						a.rewards[aCars] + b.rewards[bCars] + gamma*V[aNext][bNext]);
			return value;
		}

	int TwoStationCompany::evaluatePolicy(int iterationLimit) {
			for (int iter = 0; iter < iterationLimit; ++iter) {
				double delta = 0;
				for (int aCars = 0; aCars <= maxCars; ++aCars)
					for (int bCars = 0; bCars <= maxCars; ++bCars) {
						double oldV = V[aCars][bCars];
						V[aCars][bCars] = bellmanValue(aCars, bCars, policy[aCars][bCars]);
						delta = max(delta, abs(V[aCars][bCars] - oldV));
					}
				if (delta < .001)
					return iter;
			}
			return iterationLimit;
		}

	int TwoStationCompany::policyImprovement() {
			int changes = 0;
			for (int aCars = 0; aCars <= maxCars; ++aCars)
				for (int bCars = 0; bCars <= maxCars; ++bCars) {
					int aMaxTransfer = min(aCars, maxTransfer);
					int bMaxTransfer = min(bCars, maxTransfer);
					vector<double> qi(aMaxTransfer + bMaxTransfer + 1);
					for (int transfer = -bMaxTransfer; transfer <= aMaxTransfer; ++transfer)
						qi[bMaxTransfer + transfer] = bellmanValue(aCars, bCars, transfer);
					int bestAction = max_element(qi.begin(), qi.end()) - qi.begin() - bMaxTransfer;
					if (bestAction != policy[aCars][bCars]) {
						policy[aCars][bCars] = bestAction;
						++changes;
					}
				}
			return changes;
		}
	
	matrix<double>& TwoStationCompany::getValueMap() {
		return V;
	}

	matrix<int>& TwoStationCompany::getPolicy() {
		return policy;
	}
}