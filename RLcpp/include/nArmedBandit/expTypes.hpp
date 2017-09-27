#ifndef __N_ARMED_BANDIT_EXP_TYPES_H__
#define __N_ARMED_BANDIT_EXP_TYPES_H__

#include <cstdint>
#include <vector>
#include <utility>

namespace nArmedBandit {
	namespace expTypes {
		using namespace std;

		using real = double;
		using number = uint32_t;
		using experimentResults = pair<vector<real>, vector<real>>;
	}
}

#endif // !__N_ARMED_BANDIT_EXP_TYPES_H__
