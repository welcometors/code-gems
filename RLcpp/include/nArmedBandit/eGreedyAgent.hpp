#ifndef __N_ARMED_BANDIT_E_GREEDY_AGENT_H__
#define __N_ARMED_BANDIT_E_GREEDY_AGENT_H__

#include "expTypes.hpp"
#include <random>
#include <algorithm>
#include <chrono>

namespace nArmedBandit {
	using namespace expTypes;

	class eGreedyAgent {
		default_random_engine eng;
		uniform_int_distribution<number> choice;
		uniform_real_distribution<real> uniform;
		number bestAction;
		real bestActionValue;

		real _exploration;
		vector<number> _counts;
		vector<real> _machines;

	public:
		eGreedyAgent(number machines, real exploration);
		number action();
		void update(number machine, real reward);
	};
}

#endif // !__N_ARMED_BANDIT_E_GREEDY_AGENT_H__
