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
		}

		inline size_t action() {
			size_t best = max_element(_machines.begin(), _machines.end()) - _machines.begin();
			if (uniform(eng) < _exploration) {
				size_t chosen = choice(eng);
				return chosen < best ? chosen : chosen + 1;
			}
			return best;
		}

		inline void update(size_t machine, real reward) {
			_counts[machine]++;
			_machines[machine] += (reward - _machines[machine]) / _counts[machine];
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

	template<class T>
	auto getAverageColumnWise(const vector<vector<T>>& array2D) {
		size_t rows = array2D.size(), cols = array2D.front().size();
		vector<T> averages(cols);
		for (size_t c = 0; c < cols; c++){
			T average = T(0);
			for (size_t r = 0; r < rows; r++)
				average += array2D[r][c];
			averages[c] = average / T(rows);
		}
		return averages;
	}

	auto experiment(size_t n_runs, size_t n_machines, size_t n_steps, real exploration) {
		vector<vector<real>> rewards(n_runs, vector<real>(n_steps));
		vector<vector<real>> optimals(n_runs, vector<real>(n_steps, 0.0));

		for (size_t run = 0; run < n_runs; run++) {
			environment env(n_machines);
			agent aj(n_machines, exploration);
			for (size_t step = 0; step < n_steps; step++) {
				auto action = aj.action();
				if (action == env.optimal_action())
					optimals[run][step] = 100.0;
				aj.update(action, rewards[run][step] = env.reward(action));
			}
		}

		return make_pair(getAverageColumnWise(rewards), getAverageColumnWise(optimals));
	}
}