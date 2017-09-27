#include "nArmedBandit\eGreedyAgent.hpp"

namespace nArmedBandit {

	eGreedyAgent::eGreedyAgent(number machines, real exploration) {
		eng.seed(chrono::system_clock::now().time_since_epoch().count());
		_exploration = exploration;
		_counts.resize(machines, 0);
		_machines.resize(machines, 0.0);
		bestAction = 0;
		bestActionValue = 0.0;
		choice = uniform_int_distribution<number>(0, machines - 2);
	}

	number eGreedyAgent::action() {
		if (uniform(eng) < _exploration) {
			auto chosen = choice(eng);
			return chosen < bestAction ? chosen : chosen + 1;
		}
		return bestAction;
	}

	void eGreedyAgent::update(number machine, real reward) {
		_counts[machine]++;
		_machines[machine] += (reward - _machines[machine]) / _counts[machine];
		if (machine == bestAction && _machines[machine] < bestActionValue)
			bestActionValue = _machines[bestAction = max_element(_machines.begin(), _machines.end()) - _machines.begin()];
		else if (_machines[machine] > bestActionValue)
			bestActionValue = _machines[bestAction = machine];
	}
}
