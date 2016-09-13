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

const unsigned log2TableFor32Bit[32] = {
	 0,  9,  1, 10, 13, 21,  2, 29,
	11, 14, 16, 18, 22, 25,  3, 30,
	 8, 12, 20, 28, 15, 17, 24,  7,
	19, 27, 23,  6, 26,  5,  4, 31 };

unsigned log2_32(uint32_t value){
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	return log2TableFor32Bit[(uint32_t)(value * 0x07C4ACDD) >> 27];
}

const unsigned log2TableFor64Bit[64] = {
	63,  0, 58,  1, 59, 47, 53,  2,
	60, 39, 48, 27, 54, 33, 42,  3,
	61, 51, 37, 40, 49, 18, 28, 20,
	55, 30, 34, 11, 43, 14, 22,  4,
	62, 57, 46, 52, 38, 26, 32, 41,
	50, 36, 17, 19, 29, 10, 13, 21,
	56, 45, 25, 31, 35, 16,  9, 12,
	44, 24, 15,  8, 23,  7,  6,  5 };

unsigned log2_64(uint64_t value){
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	value |= value >> 32;
	return log2TableFor64Bit[((uint64_t)((value - (value >> 1)) * 0x07EDD5E59A4E28C2)) >> 58];
}