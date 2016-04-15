using namespace std;

#define natural unsigned long long
#define Modulus 1000000007ULL

#define modADD(x, y) (((x) + (y)) % Modulus)
#define modSUB(x, y) (((x) + Modulus - (y)) % Modulus)
#define modMUL(x, y) (((x) * (y)) % Modulus)

#pragma region "POWER"

#define modINV(x) modPOW(x, Modulus-2)
#define modDIV(x, y) (((x) * modINV(y)) % Modulus)

natural modPOW(natural i, natural j){
	if (!j)
		return 1;

	if (i < 2)
		return i;

	natural p = 1;

	while (j){
		if (j & 1)
			p = (p * i) % Modulus;
		i = (i * i) % Modulus;
		j >>= 1;
	}

	return p;
}

#pragma endregion

#pragma region "DIGIT POWER"

int pow2powModulus[33] = {
	1, 2, 4, 16, 256, 65536, 294967268, 582344008, 279632277, 792845266, 418385479,
	812734592, 409643880, 246797651, 112754241, 774491455, 669157962, 973586826,
	125655169, 385894014, 998632880, 36221046, 164150368, 125918023, 405241093,
	306292255, 816280417, 513497891, 215687307, 75265151, 915398907, 71108578,
	829787081 };

natural powerOf2(int j){
	natural p = 1;

	for (int i = 1; j; i++, j >>= 1){
		if (j & 1)
			p = (p * pow2powModulus[i]) % Modulus;
	}

	return p;
}

#pragma endregion

#pragma region "INCREMENTAL FACTORIAL"

#include <vector>

vector<natural> factorialTableModulus = { 1 };
natural modFACT(int v){
	int n = factorialTableModulus.size();
	if ( n > v)
		return factorialTableModulus[v];

	natural l = factorialTableModulus.back();
	while (n <= v){
		l = (l * n) % Modulus;
		factorialTableModulus.push_back(l);
		n++;
	}
	return factorialTableModulus[v];
}

#pragma endregion

#pragma region "BINOMIAL COFFICIENT TABLE"

#include <memory>
#define nCrTableMaxN 10000
unique_ptr<natural[]> nCrTableModulus(nullptr);

void nCrTableModulusInit(){
	int size = (nCrTableMaxN + 2) >> 1;
	size *= (size + 1);

	unique_ptr<natural[]> nCrTableLocal(new natural[size]);
	nCrTableModulus = std::move(nCrTableLocal);
	nCrTableModulus[0] = nCrTableModulus[1] = 1;

	natural *ptr = &nCrTableModulus[2], *lptr = &nCrTableModulus[1];

	for (int i = 2; i <= nCrTableMaxN; i++){
		*ptr++ = 1;
		for (int j = 1, l = ((i + 2) >> 1) - 1 + (i & 1); j < l; j++, lptr++){
			*ptr = *lptr + *(lptr + 1);
			if (*ptr > Modulus)
				*ptr -= Modulus;
			*ptr++;
		}
		if (!(i & 1)){
			*ptr = *lptr + *lptr;
			if (*ptr > Modulus)
				*ptr -= Modulus;
			*ptr++;
		}
		lptr++;
	}
}

natural modNCR(int n, int r){
	if (r > n)
		return 0;
	else if (r > n - r)
		r = n - r;

	if (n < 2 || r == 0)
		return 1;

	int k = (n + 2) >> 1;
	return nCrTableModulus[(k - 1) * k + (n & 1) * k + r];
}

#pragma endregion

#pragma region "BINOMIAL COFFICIENT FACTORIAL"

natural modNCR(int n, int r){
	if (r > n)
		return 0;
	else if (r > n - r)
		r = n - r;

	if (n < 2 || r == 0)
		return 1;

	if (r == 1)
		return n;

	return modDIV(modFACT(n), modMUL(modFACT(r), modFACT(n - r)));
}

#pragma endregion