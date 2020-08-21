#ifndef __N_ARMED_BANDIT_ENVIRONMENT_H__
#define __N_ARMED_BANDIT_ENVIRONMENT_H__

#include "expTypes.hpp"
#include <random>
#include <chrono>

namespace nArmedBandit {
	using namespace expTypes;

	class environment {
		default_random_engine eng;
		normal_distribution<real> gaussian;
		vector<real> _machines;
		number _optimal;

	public:
		explicit environment(number machines);

		inline number optimal_action() {
			return _optimal;
		}

		real reward(number machine);
	};
}

#endif // !__N_ARMED_BANDIT_ENVIRONMENT_H__

