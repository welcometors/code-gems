#include <cstdint>

const unsigned char BitsCountTable256[256] = {
#define B2(n) n,     n+1,     n+1,     n+2
#define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
	B6(0), B6(1), B6(1), B6(2)
};

inline int bitCount32(uint32_t v){
	unsigned char *p = (unsigned char *)&v;
	return BitsCountTable256[p[0]] + BitsCountTable256[p[1]] +
		BitsCountTable256[p[2]] + BitsCountTable256[p[3]];
}

inline int bitCount64(uint64_t v){
	unsigned char *p = (unsigned char *)&v;
	return BitsCountTable256[p[0]] + BitsCountTable256[p[1]] +
		BitsCountTable256[p[2]] + BitsCountTable256[p[3]] +
		BitsCountTable256[p[4]] + BitsCountTable256[p[5]] +
		BitsCountTable256[p[6]] + BitsCountTable256[p[7]];
}