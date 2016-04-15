//#include <type_traits>

// this class has exact same performance as raw calculation
#pragma region "CLASS"

template<class T, T Modulus>
class Modular {
	static_assert(Modulus >= 2, "Modular arithmatic with base less than 2 is not supported");

private:
	T value;

public:
	Modular(const T &init);
	Modular(const Modular& other);
	Modular& operator= (const Modular &other);
	Modular operator - () const;
	T Value() const;
	Modular inv() const;

	Modular& operator += (const Modular &a);
	Modular& operator -= (const Modular &a);
	Modular& operator *= (const Modular &a);
	Modular& operator /= (const Modular &a);

	template<class T1, T1 Modulus1>
	friend Modular<T1, Modulus1> operator + (const Modular<T1, Modulus1> &a, const Modular<T1, Modulus1> &b);

	template<class T1, T1 Modulus1>
	friend Modular<T1, Modulus1> operator - (const Modular<T1, Modulus1> &a, const Modular<T1, Modulus1> &b);

	template<class T1, T1 Modulus1>
	friend Modular<T1, Modulus1> operator * (const Modular<T1, Modulus1> &a, const Modular<T1, Modulus1> &b);

	template<class T1, T1 Modulus1>
	friend Modular<T1, Modulus1> operator / (const Modular<T1, Modulus1> &a, const Modular<T1, Modulus1> &b);

	template<class T1, T1 Modulus1>
	friend Modular<T1, Modulus1> pow(Modular<T1, Modulus1> i, T1 j);

};

template<class T, T Modulus>
Modular<T, Modulus>::Modular(const T &init) : value(init % Modulus) { if (value < 0)value += Modulus; }

template<class T, T Modulus>
Modular<T, Modulus>::Modular(const Modular& other) : value(other.value) {}

template<class T, T Modulus>
Modular<T, Modulus>& Modular<T, Modulus>::operator= (const Modular<T, Modulus> &other) { value = other.value; return *this; }

template<class T, T Modulus>
Modular<T, Modulus> Modular<T, Modulus>::operator - () const {
	if (value)
		return Modular(Modulus - value);
	return Modular(0);
}

template<class T, T Modulus>
Modular<T, Modulus>& Modular<T, Modulus>::operator += (const Modular<T, Modulus> &a) {
	value += a.value;
	if (value >= Modulus)
		value -= Modulus;
	return *this;
}

template<class T, T Modulus>
Modular<T, Modulus>& Modular<T, Modulus>::operator -= (const Modular<T, Modulus> &a) {
	value += Modulus - a.value;
	if (value >= Modulus)
		value -= Modulus;
	return *this;
}

template<class T, T Modulus>
Modular<T, Modulus>& Modular<T, Modulus>::operator *= (const Modular<T, Modulus> &a) {
	value *= a.value;
	if (value >= Modulus)
		value %= Modulus;
	return *this;
}

template<class T, T Modulus>
Modular<T, Modulus>& Modular<T, Modulus>::operator /= (const Modular<T, Modulus> &a) {
	*this *= a.inv();
	if (value >= Modulus)
		value %= Modulus;
	return *this;
}

template<class T, T Modulus>
T Modular<T, Modulus>::Value() const {
	return value;
}

template<class T, T Modulus>
Modular<T, Modulus> operator + (const Modular<T, Modulus> &a, const Modular<T, Modulus> &b) {
	Modular<T, Modulus> s(a.value);
	return s += b;
}

template<class T, T Modulus>
Modular<T, Modulus> operator - (const Modular<T, Modulus> &a, const Modular<T, Modulus> &b) {
	Modular<T, Modulus> s(a.value);
	return s -= b;
}

template<class T, T Modulus>
Modular<T, Modulus> operator * (const Modular<T, Modulus> &a, const Modular<T, Modulus> &b) {
	Modular<T, Modulus> s(a.value);
	return s *= b;
}

template<class T, T Modulus>
Modular<T, Modulus> operator / (const Modular<T, Modulus> &a, const Modular<T, Modulus> &b) {
	Modular<T, Modulus> s(a.value);
	return s /= b;
}

template<class T, T Modulus>
Modular<T, Modulus> pow(Modular<T, Modulus> i, T j) {
	if (j == T(0))
		return Modular<T, Modulus>(1);

	if (i.value < T(2))
		return i;

	Modular<T, Modulus> p(1);
	while (j) {
		if (j & 1)
			p *= i;
		i *= i;
		j >>= 1;
	}
	return p;
}

template<class T, T Modulus>
Modular<T, Modulus> Modular<T, Modulus>::inv() const {
	return pow<T, Modulus>(*this, Modulus - 2);
}

#pragma endregion

typedef unsigned long long natural;
constexpr natural Limit = 1000000007ULL;
typedef Modular<natural, Limit> modular;

#pragma region "INCREMENTAL FACTORIAL"
#include <vector>

std::vector<modular> factorialTableModulus = { 1 };
modular fact(size_t v) {
	auto n = factorialTableModulus.size();
	if (n > v)
		return factorialTableModulus[v];

	auto l = factorialTableModulus.back();
	while (n <= v) {
		l *= n;
		factorialTableModulus.push_back(l);
		n++;
	}
	return l;
}

modular nCr(size_t n, size_t r) {
	if (r > n)
		return 0;
	else if (r > n - r)
		r = n - r;

	if (n < 2 || r == 0)
		return 1;

	if (r == 1)
		return n;

	return fact(n) / (fact(r) * fact(n - r));
}

#pragma endregion