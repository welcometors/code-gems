#ifndef __N_ARMED_BANDIT_SOFTMAX_AGENT_H__
#define __N_ARMED_BANDIT_SOFTMAX_AGENT_H__

#include "expTypes.hpp"
#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>

namespace nArmedBandit {
	using namespace expTypes;

	class softmaxAgent {
		default_random_engine eng;
		uniform_real_distribution<real> uniform;

		real _temperature;
		vector<number> _counts;
		vector<real> _machines;
		vector<real> exponent;
		real exponentSum;
		size_t updates;

	public:
		softmaxAgent(number machines, real temperature);
		number action();
		void update(number machine, real reward);
	};
}

#endif // !__N_ARMED_BANDIT_SOFTMAX_AGENT_H__
