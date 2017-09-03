#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

namespace eGreedy {
	using namespace std;
	using real = double;

	class agent {
		default_random_engine eng;
		normal_distribution<real> gaussian;
		uniform_int_distribution<size_t> choice;
		uniform_real_distribution<real> uniform;
		size_t best;
		real bestValue;

		real _exploration;
		vector<uint32_t> _counts;
		vector<real> _machines;

	public:
		agent(size_t machines, real exploration) {
			eng.seed(chrono::system_clock::now().time_since_epoch().count());
			_exploration = exploration;
			_counts.resize(machines, 0);
			_machines.resize(machines, 0.0);
			choice = uniform_int_distribution<size_t>(0, machines - 2);
			best = 0;
			bestValue = 0.0;
		}

		inline size_t action() {
			if (uniform(eng) < _exploration) {
				size_t chosen = choice(eng);
				return chosen < best ? chosen : chosen + 1;
			}
			return best;
		}

		inline void update(size_t machine, real reward) {
			_counts[machine]++;
			_machines[machine] += (reward - _machines[machine]) / _counts[machine];
			if (_machines[machine] > bestValue) {
				bestValue = _machines[machine];
				best = machine;
			}
		}
	};

	class environment {
		default_random_engine eng;
		normal_distribution<real> gaussian;
		vector<real> _machines;
		size_t _optimal;

	public:
		explicit environment(size_t machines) {
			eng.seed(chrono::system_clock::now().time_since_epoch().count());
			_machines.resize(machines, 0.0);
			for (auto& machine : _machines)
				machine = gaussian(eng);
			_optimal = max_element(_machines.begin(), _machines.end()) - _machines.begin();
		}

		inline size_t optimal_action() {
			return _optimal;
		}

		inline real reward(size_t machine) {
			return _machines[machine] + gaussian(eng);
		}
	};
	
	auto experiment(size_t n_runs, size_t n_machines, size_t n_steps, real exploration) {
		vector<real> rewards(n_steps, 0.0);
		vector<real> optimals(n_steps, 0.0);

		for (size_t run = 0; run < n_runs; run++) {
			environment env(n_machines);
			agent aj(n_machines, exploration);
			for (size_t step = 0; step < n_steps; step++) {
				auto action = aj.action();
				if (action == env.optimal_action())
					optimals[step] += 100.0;
				auto reward = env.reward(action);
				rewards[step] += reward;
				aj.update(action, reward);
			}
		}

		for (auto& x : rewards)
			x /= n_runs;

		for (auto& x : optimals)
			x /= n_runs;

		return make_pair(rewards, optimals);
	}
}