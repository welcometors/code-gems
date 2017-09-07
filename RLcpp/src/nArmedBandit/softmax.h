#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <thread>
#include <future>
#include <chrono>

namespace softmax {
	using namespace std;
	using real = double;
	using number = uint32_t;

	class environment {
		default_random_engine eng;
		normal_distribution<real> gaussian;
		vector<real> _machines;
		number _optimal;

	public:
		explicit environment(number machines) {
			eng.seed(chrono::system_clock::now().time_since_epoch().count());
			_machines.resize(machines);
			for (auto& machine : _machines)
				machine = gaussian(eng);
			_optimal = max_element(_machines.begin(), _machines.end()) - _machines.begin();
		}

		inline auto optimal_action() {
			return _optimal;
		}

		inline auto reward(number machine) {
			return _machines[machine] + gaussian(eng);
		}
	};

	class agent {
		default_random_engine eng;
		uniform_real_distribution<real> uniform;

		real _temperature;
		vector<number> _counts;
		vector<real> _machines;
		vector<real> exponent;
		real exponentSum;
		size_t updates;

	public:
		agent(number machines, real temperature) {
			eng.seed(chrono::system_clock::now().time_since_epoch().count());
			_temperature = temperature;
			_counts.resize(machines, 0);
			_machines.resize(machines, 0.0);
			exponent.resize(machines, 1.0);
			exponentSum = machines;
			updates = 0;
		}

		inline size_t action() {
			const auto n = _machines.size();
			real cdf = exponent[0], choice = uniform(eng)*exponentSum;
			for (size_t i = 1; i < n; i++) {
				if (cdf > choice)
					return i - 1;
				cdf += exponent[i];
			}
			return n - 1;
		}

		inline void update(number machine, real reward) {
			_counts[machine]++;
			_machines[machine] += (reward - _machines[machine]) / _counts[machine];
			exponentSum -= exponent[machine];
			exponent[machine] = exp(_machines[machine] / _temperature);
			if (!(++updates & 0xFFF))
				// refresh needed as floating point operations my loose precision
				exponentSum = accumulate(exponent.begin(), exponent.end(), 0.0);
			else
				exponentSum += exponent[machine];
		}
	};

	auto conductExperiment(number n_runs, number n_machines, number n_steps, real temperature) {
		vector<real> rewards(n_steps, 0.0);
		vector<real> optimals(n_steps, 0.0);

		for (number run = 0; run < n_runs; run++) {
			environment env(n_machines);
			agent aj(n_machines, temperature);
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
	
	auto conductExperiments(number n_runs, number n_machines, number n_steps, real temperature) {
		unsigned n_threads = thread::hardware_concurrency();

		if (n_threads)
			n_threads--; // don't use 100% cpu

		if (n_threads < 2U)
			return conductExperiment(n_runs, n_machines, n_steps, temperature);

		vector<future<pair<vector<real>, vector<real>>>> experiments;
		for (unsigned i = 0; i < n_threads; i++) {
			number runs = n_runs / n_threads;
			if (!i)	// put extra runs in first thread
				runs += n_runs % n_threads;
			experiments.emplace_back(async(conductExperiment, runs, n_machines, n_steps, temperature));
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
}