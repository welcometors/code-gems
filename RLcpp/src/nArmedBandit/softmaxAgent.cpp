#include "nArmedBandit\softmaxAgent.hpp"

namespace nArmedBandit {

	softmaxAgent::softmaxAgent(number machines, real temperature) {
		eng.seed(chrono::system_clock::now().time_since_epoch().count());
		_temperature = temperature;
		_counts.resize(machines, 0);
		_machines.resize(machines, 0.0);
		exponent.resize(machines, 1.0);
		exponentSum = machines;
		updates = 0;
	}

	number softmaxAgent::action() {
		const auto n = _machines.size();
		real cdf = exponent[0], choice = uniform(eng)*exponentSum;
		for (size_t i = 1; i < n; i++) {
			if (cdf > choice)
				return i - 1;
			cdf += exponent[i];
		}
		return n - 1;
	}

	void softmaxAgent::update(number machine, real reward) {
		_counts[machine]++;
		_machines[machine] += (reward - _machines[machine]) / _counts[machine];
		exponentSum -= exponent[machine];
		exponent[machine] = exp(_machines[machine] / _temperature);
		if (!(++updates & 0xFFF))
			// refresh needed as floating point operations my loose precision overtime
			exponentSum = accumulate(exponent.begin(), exponent.end(), 0.0);
		else
			exponentSum += exponent[machine];
	}
}
