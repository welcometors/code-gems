using namespace std;

#pragma region "PRIME VECTOR"

#include <vector>
#include <memory>
#include <cstring>

void populatePrimes(int limit, vector<unsigned> &primes){
	int sqrtLimit = (int)sqrt(limit + 1) + 1, k = 0;
	unique_ptr<int[]> isPrime(new int[limit + 1]);

	isPrime[0] = isPrime[1] = 0;
	std::memset(&isPrime[2], 1, sizeof(int)*(limit - 1));
	primes.clear();

	for (int i = 2; i <= sqrtLimit; i++){
		if (!isPrime[i])
			continue;

		primes.push_back(i);

		for (int j = i + i; j <= limit; j += i)
			isPrime[j] = false;
	}

	for (int i = sqrtLimit + 1; i <= limit; i++){
		if (isPrime[i])
			primes.push_back(i);
	}
}

vector<unsigned> populatePrimes(int limit){
	vector<unsigned> primes;
	int sqrtLimit = (int)sqrt(limit + 1) + 1, k = 0;
	unique_ptr<int[]> isPrime(new int[limit + 1]);

	isPrime[0] = isPrime[1] = 0;
	std::memset(&isPrime[2], 1, sizeof(int)*(limit - 1));
	primes.clear();

	for (int i = 2; i <= sqrtLimit; i++){
		if (!isPrime[i])
			continue;

		primes.push_back(i);

		for (int j = i + i; j <= limit; j += i)
			isPrime[j] = false;
	}

	for (int i = sqrtLimit + 1; i <= limit; i++){
		if (isPrime[i])
			primes.push_back(i);
	}

	return primes;
}

int totient(int n, const vector<unsigned> &primes){
	int l = 0, r = primes.size() - 1;

	while (l <= r){
		int m = (l + r) >> 1;

		if (n == primes[m])
			return m + 1;
		else if (n < primes[m])
			r = m - 1;
		else
			l = m + 1;
	}

	return l;
}

template<class T>
T totient(T n, const vector<unsigned> &primes) {
	T tot = 1;
	for (auto prime : primes) {
		unsigned power = 0;
		T powerMul = 1;
		while (n % prime == 0) {
			if (power++)
				powerMul *= prime;
			n /= prime;
		}
		if (power)
			tot *= powerMul * (prime - 1);
		if (n == 1)
			break;
	}
	if (n > 1)
		tot *= n - 1;
	return tot;
}

unique_ptr<int[]> populateMobius(int limit) {
	int sqrtLimit = (int)sqrt(limit) + 1;
	unique_ptr<int[]> sieve(new int[limit + 1]);
	for (int i = 1; i <= limit; i++)
		sieve[i] = 1;

	for (int i = 2; i <= sqrtLimit; i++) {
		if (sieve[i] == 1) {
			for (int j = i; j <= limit; j += i)
				sieve[j] *= -i;
			for (int j = i * i; j <= limit; j += i * i)
				sieve[j] = 0;
		}
	}

	for (int i = 2; i <= limit; i++)
		if (sieve[i] == i)
			sieve[i] = 1;
		else if (sieve[i] == -i)
			sieve[i] = -1;
		else if (sieve[i] < 0)
			sieve[i] = 1;
		else if (sieve[i] > 0)
			sieve[i] = -1;

	return sieve;
}

unique_ptr<int[]> populateTotient(int limit) {
	int sqrtLimit = (int)sqrt(limit) + 1;

	unique_ptr<int[]> sieve(new int[limit + 1]);
	sieve[0] = 1;
	for (int i = 1; i <= limit; i++)
		sieve[i] = i;

	for (int i = 2; i <= limit; i++)
		if (sieve[i] == i)
			for (int j = i; j <= limit; j += i)
				sieve[j] -= sieve[j] / i;

	return sieve;
}

#pragma endregion

#pragma region "Factors"

const unsigned primes[] = { // some primes
	2,   3,   5,   7,  11,  13,  17,  19,  23,  29,  31,  37,  41,
	43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97, 101,
	103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
	173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
	241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313 };

template<class T>
T getNumOfFactors(T n) {
	T factors = 1;
	for (auto prime : primes) {
		natural power = 0;
		while (n % prime == 0) {
			n /= prime;
			power++;
		}
		if (power)
			factors *= power + 1;
		if (n == 1)
			break;
	}
	if (n > 1)
		factors *= 2;
	return factors;
}

template<class T>
vector<T> getAllFactors(T n) {
	vector<T> factors = { 1 };
	for (auto prime : primes) {
		T power = 1;
		size_t nFactors = factors.size();
		while (n % prime == 0) {
			power *= prime;
			for (size_t i = 0; i < nFactors; i++)
				factors.push_back(factors[i] * power);
			n /= prime;
		}
		if (n == 1)
			break;
	}
	if (n > 1)
		for (size_t i = 0, nFactors = factors.size(); i < nFactors; i++)
			factors.push_back(factors[i] * n);
	return factors;
}

#pragma endregion

#pragma region "Miller-Rabin"

namespace DeterministicMillerRabin {
	// Deterministic up to 341,550,071,728,321
	
	uint64_t modMul(uint64_t i, uint64_t j, uint64_t k) {
		if (i >= (1ULL << 32) || j >= (1ULL << 32)) {
			if (i >= k)
				i %= k;
			if (j >= k)
				j %= k;
			// Russian peasant multiplication
			uint64_t r = 0;
			while (j > 0) {
				if (j & 1) {
					r += i;
					if (r >= k)
						r -= k;
				}
				i <<= 1;
				if (i >= k)
					i -= k;
				j >>= 1;
			}
			return r;
		}
		uint64_t r = i*j;
		if (r >= k)
			r %= k;
		return r;
	}

	uint64_t modPower(uint64_t a, uint64_t n, uint64_t mod) {
		uint64_t power = a;
		uint64_t result = 1;
		while (n) {
			if (n & 1)
				result = modMul(result, power, mod);
			power = modMul(power, power, mod);
			n >>= 1;
		}
		return result;
	}

	// n-1 = 2^s * d with d odd by factoring powers of 2 from n-1
	bool witness(uint64_t n, uint64_t s, uint64_t d, uint64_t a) {
		uint64_t x = modPower(a, d, n);
		uint64_t y;
		while (s) {
			y = modMul(x, x, n);
			if (y == 1 && x != 1 && x != n - 1)
				return false;
			x = y;
			--s;
		}
		if (y != 1)
			return false;
		return true;
	}

	bool isPrime(uint64_t n) {
		if (((!(n & 1)) && n != 2) || (n < 2) || (n % 3 == 0 && n != 3))
			return false;
		if (n <= 3)
			return true;

		uint64_t d = n / 2;
		uint64_t s = 1;
		while (!(d & 1)) {
			d /= 2;
			++s;
		}

		if (n < 1373653ULL)
			return witness(n, s, d, 2) && witness(n, s, d, 3);
		if (n < 9080191ULL)
			return witness(n, s, d, 31) && witness(n, s, d, 73);
		if (n < 4759123141ULL)
			return witness(n, s, d, 2) && witness(n, s, d, 7) && witness(n, s, d, 61);
		if (n < 1122004669633ULL)
			return witness(n, s, d, 2) && witness(n, s, d, 13) && witness(n, s, d, 23) && witness(n, s, d, 1662803);
		if (n < 2152302898747ULL)
			return witness(n, s, d, 2) && witness(n, s, d, 3) && witness(n, s, d, 5) && witness(n, s, d, 7) && witness(n, s, d, 11);
		if (n < 3474749660383ULL)
			return witness(n, s, d, 2) && witness(n, s, d, 3) && witness(n, s, d, 5) && witness(n, s, d, 7) && witness(n, s, d, 11) && witness(n, s, d, 13);
		return witness(n, s, d, 2) && witness(n, s, d, 3) && witness(n, s, d, 5) && witness(n, s, d, 7) && witness(n, s, d, 11) && witness(n, s, d, 13) && witness(n, s, d, 17);
	}
}

#pragma endregion

#pragma region "Forisek Jancina 32Bit"
namespace ForisekJancina32 {
	// ref  Fast Primality Testing for Integers That Fit into a Machine Word
	uint16_t bases[] = { 15591,2018,166,7429,8064,16045,10503,4399,1949,1295,2776,3620,560,3128,5212,
		2657,2300,2021,4652,1471,9336,4018,2398,20462,10277,8028,2213,6219,620,3763,4852,5012,3185,
		1333,6227,5298,1074,2391,5113,7061,803,1269,3875,422,751,580,4729,10239,746,2951,556,2206,
		3778,481,1522,3476,481,2487,3266,5633,488,3373,6441,3344,17,15105,1490,4154,2036,1882,1813,
		467,3307,14042,6371,658,1005,903,737,1887,7447,1888,2848,1784,7559,3400,951,13969,4304,177,41,
		19875,3110,13221,8726,571,7043,6943,1199,352,6435,165,1169,3315,978,233,3003,2562,2994,10587,
		10030,2377,1902,5354,4447,1555,263,27027,2283,305,669,1912,601,6186,429,1930,14873,1784,1661,
		524,3577,236,2360,6146,2850,55637,1753,4178,8466,222,2579,2743,2031,2226,2276,374,2132,813,
		23788,1610,4422,5159,1725,3597,3366,14336,579,165,1375,10018,12616,9816,1371,536,1867,10864,
		857,2206,5788,434,8085,17618,727,3639,1595,4944,2129,2029,8195,8344,6232,9183,8126,1870,3296,
		7455,8947,25017,541,19115,368,566,5674,411,522,1027,8215,2050,6544,10049,614,774,2333,3007,
		35201,4706,1152,1785,1028,1540,3743,493,4474,2521,26845,8354,864,18915,5465,2447,42,4511,
		1660,166,1249,6259,2553,304,272,7286,73,6554,899,2816,5197,13330,7054,2818,3199,811,922,350,
		7514,4452,3449,2663,4708,418,1621,1171,3471,88,11345,412,1559,194 };

	bool is_SPRP(uint32_t n, uint32_t a) {
		uint32_t d = n - 1, s = 0;
		while ((d & 1) == 0) ++s, d >>= 1;
		uint64_t cur = 1, pw = d;
		while (pw) {
			if (pw & 1) cur = (cur*a) % n;
			a = ((uint64_t)a*a) % n;
			pw >>= 1;
		}
		if (cur == 1) return true;
		for (uint32_t r = 0; r<s; r++) {
			if (cur == n - 1) return true;
			cur = (cur*cur) % n;
		}
		return false;
	}

	bool isPrime(uint32_t x) {
		if (x == 2 || x == 3 || x == 5 || x == 7) return true;
		if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0) return false;
		if (x<121) 
			return x > 1;

		uint64_t h = x;
		h = ((h >> 16) ^ h) * 0x45d9f3b;
		h = ((h >> 16) ^ h) * 0x45d9f3b;
		h = ((h >> 16) ^ h) & 255;
		return is_SPRP(x, bases[h]);
	}
}

#pragma endregion