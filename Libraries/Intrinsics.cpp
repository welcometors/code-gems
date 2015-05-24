using namespace std;

#ifdef _MSC_VER
#include <intrin.h>
#include <cstdint>

uint32_t __inline __builtin_ctz(uint32_t value){
	unsigned long trailing_zero = 0;

	if (_BitScanForward(&trailing_zero, value))
		return trailing_zero;

	// Undefined, 32 seems better than 0
	return 32;
}

uint32_t __inline __builtin_clz(uint32_t value){
	unsigned long leading_zero = 0;

	if (_BitScanReverse(&leading_zero, value))
		return 31 - leading_zero;

	// Undefined, 32 seems better than 0
	return 32;
}
#endif

#ifdef _MSC_VER
uint32_t popcnt(uint32_t x){ return _mm_popcnt_u32(x); }
#else
uint32_t popcnt(uint32_t x){ return __builtin_popcount(x); }
#endif