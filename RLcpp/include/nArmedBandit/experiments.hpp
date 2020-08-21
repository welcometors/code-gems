#ifndef __N_ARMED_BANDIT_EXPERIMENTS_H__
#define __N_ARMED_BANDIT_EXPERIMENTS_H__

#include "expTypes.hpp"
#include "environment.hpp"
#include "eGreedyAgent.hpp"
#include "softmaxAgent.hpp"
#include <thread>
#include <future>

namespace nArmedBandit {

	namespace experiments {

		experimentResults eGreedy(
			number n_runs, number n_machines, number n_steps, real exploration);

		experimentResults softmax(
			number n_runs, number n_machines, number n_steps, real temperature);
	}
}

#endif // !__N_ARMED_BANDIT_EXPERIMENTS_H__
