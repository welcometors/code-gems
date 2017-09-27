#include "nArmedBandit\environment.hpp"

namespace nArmedBandit {
	environment::environment(number machines) {
		eng.seed(chrono::system_clock::now().time_since_epoch().count());
		_machines.resize(machines);
		for (auto& machine : _machines)
			machine = gaussian(eng);
		_optimal = max_element(_machines.begin(), _machines.end()) - _machines.begin();
	}

	real environment::reward(number machine) {
		return _machines[machine] + gaussian(eng);
	}
}


