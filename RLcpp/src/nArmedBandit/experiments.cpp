#include "nArmedBandit\experiments.hpp"

namespace nArmedBandit {
	
	template<class Agent>
	experimentResults conductExperiment(number n_runs, number n_machines, number n_steps, real param) {
		vector<real> rewards(n_steps, 0.0);
		vector<real> optimals(n_steps, 0.0);

		for (number run = 0; run < n_runs; run++) {
			environment env(n_machines);
			Agent aj(n_machines, param);
			for (number step = 0; step < n_steps; step++) {
				auto action = aj.action();
				if (action == env.optimal_action())
					optimals[step] += 100.0;
				auto reward = env.reward(action);
				rewards[step] += reward;
				aj.update(action, reward);
			}
		}

		return make_pair(rewards, optimals);
	}

	template<class Agent>
	experimentResults conductExperiments(number n_runs, number n_machines, number n_steps, real param) {
		unsigned n_threads = thread::hardware_concurrency();

		if (n_threads)
			n_threads--; // don't use 100% cpu

		if (n_threads < 2U)
			return conductExperiment<Agent>(n_runs, n_machines, n_steps, param);

		vector<future<pair<vector<real>, vector<real>>>> experiments;
		for (unsigned i = 0; i < n_threads; i++) {
			number runs = n_runs / n_threads;
			if (!i)	// put extra runs in first thread
				runs += n_runs % n_threads;
			experiments.emplace_back(async(launch::async, conductExperiment<Agent>, runs, n_machines, n_steps, param));
		}

		vector<real> rewards(n_steps, 0.0);
		vector<real> optimals(n_steps, 0.0);

		for (auto& e : experiments) {
			auto result = e.get();
			for (number step = 0; step < n_steps; step++) {
				rewards[step] += result.first[step];
				optimals[step] += result.second[step];
			}
		}

		for (auto& x : rewards)
			x /= n_runs;

		for (auto& x : optimals)
			x /= n_runs;

		return make_pair(rewards, optimals);
	}

	namespace experiments {

		experimentResults eGreedy(
			number n_runs, number n_machines, number n_steps, real exploration) {
			return conductExperiments<eGreedyAgent>(n_runs, n_machines, n_steps, exploration);
		}

		experimentResults softmax(
			number n_runs, number n_machines, number n_steps, real temperature) {
			return conductExperiments<softmaxAgent>(n_runs, n_machines, n_steps, temperature);
		}
	}
}


