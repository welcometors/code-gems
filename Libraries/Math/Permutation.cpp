typedef unsigned long long natural;

natural factorial[] = { 1,1,2,6,24,120,720,5040,40320,362880,3628800,
39916800,479001600,6227020800,87178291200,1307674368000,20922789888000,
355687428096000,6402373705728000,121645100408832000
};

template<class T>
T getNthPermutation(const T& s, natural n) {
	size_t l = s.length() - 1;
	T p = s;
	for (size_t i = 0; i < l; i++) {
		natural j = n / factorial[l - i];
		if (j) { // move jth value to front
			auto val = p[j + i];
			for (size_t k = j + i; k > i; k--)
				p[k] = p[k - 1];
			p[i] = val;
			n -= j * factorial[l - i];
		}
	}
	return p;
}