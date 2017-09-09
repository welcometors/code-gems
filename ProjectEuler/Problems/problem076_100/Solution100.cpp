// https://projecteuler.net/problem=100
/*
Problem 100

Arranged probability

If a box contains twenty-one coloured discs, composed of fifteen blue discs
and six red discs, and two discs were taken at random, it can be seen that
the probability of taking two blue discs, P(BB) = (15/21)×(14/20) = 1/2.

The next such arrangement, for which there is exactly 50% chance of taking two
blue discs at random, is a box containing eighty-five blue discs and thirty-five
red discs.

By finding the first arrangement to contain over 10^12 = 1,000,000,000,000 discs
in total, determine the number of blue discs that the box would contain.

Solution:
For current case:
https://www.alpertron.com.ar/QUAD.HTM

For generalized case i.e. probability = p/q
Solving General Pell's Equation
http://www.jpr2718.org/FundSoln.pdf

*/

#include <iostream>
#include <tuple>
#include <set>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <chrono>
using namespace std;

using ll = long long;

template<class T, class V>
ostream& operator << (ostream& out, const pair<T, V>& val) {
	out << val.first << ' ' << val.second;
	return out;
}

template<class T>
typename std::enable_if<std::is_unsigned<T>::value, T>::type GCD(T u, T v) {
	int shift;
	if (u == 0) return v;
	if (v == 0) return u;

	for (shift = 0; ((u | v) & 1) == 0; ++shift) {
		u >>= 1;
		v >>= 1;
	}

	while ((u & 1) == 0)
		u >>= 1;

	do {
		while ((v & 1) == 0)
			v >>= 1;
		if (u > v) {
			T t = v;
			v = u;
			u = t;
		}
		v = v - u;
	} while (v != 0);

	return u << shift;
}

// x ^ 2 - n.y ^ 2 = 1
auto pell(ll n) {
	ll sqrtn = sqrt(n);
	if (sqrtn*sqrtn == n)
		return make_pair(ll(0), ll(0));

	ll p = 0, q = 1, a = sqrtn;
	ll x0 = 1, y0 = 0, x1 = a, y1 = 1;
	while (x1*x1 - n*y1*y1 != 1) {
		p = q*a - p;
		q = (n - p*p) / q;
		a = (sqrtn + p) / q;
		tie(x0, y0, x1, y1) = make_tuple(x1, y1, a*x1 + x0, a*y1 + y0);
	}
	return make_pair(x1, y1);
}

// x ^ 2 - p.y ^ 2 = q
auto generalPell(ll p, ll q) {
	vector<pair<ll, ll>> solns;

	ll m, n;
	tie(m, n) = pell(p);

	if (m) {
		ll ubx = sqrt((m + 1.0)*q / 2.0);
		ll uby = sqrt(q / (2.0 * (m + 1.0)))*n;
		ll nx = 0, ny = 0;
		for (ll y = 0; y <= uby; y++)
			for (ll x = 1; x <= ubx; x++)
				if (x*x - p*y*y == q) {
					solns.push_back({ x, y });
					solns.push_back({ -x, y });
				}
	}

	return make_tuple(solns, m, n);
}

auto solve(ll p, ll q, ll lim) {
	ll g = GCD<unsigned long long>(q, p);
	p /= g;
	q /= g;
	set<pair<ll, ll>> solns;

	ll m, n;
	vector<pair<ll, ll>> primitive;
	tie(primitive, m, n) = generalPell(p*q, q*(q - p));
	if (!primitive.size())
		return solns;

	for (auto& base : primitive) {
		ll x = base.first, y = base.second;
		while (abs(y) < 2 * lim) {
			if (x%q == 0) {
				ll t = abs(y) + 1, b = abs(x / q) + 1;
				if (!(t & 1) && !(b & 1)) {
					t >>= 1;
					b >>= 1;
					if (1 < t && t <= lim)
						solns.insert({ b, t });
				}
			}
			tie(x, y) = make_pair(m*x + n*p*q*y, n*x + m*y);
		}
	}
	return solns;
}

auto solveBF(ll p, ll q, ll lim) {
	set<pair<ll, ll>> solns;

	for (ll b = 1; b <= lim; b++)
		for (ll t = b + 1; t <= lim; t++)
			if (q*b*(b - 1) == p*t*(t - 1))
				solns.insert({ b, t });

	return solns;
}

void test(int t, int r, ll l) {
	int f = 0;
	for (int i = 0; i < t; i++) {
		ll q = 2 + rand() % (r - 1);
		ll p = 1 + rand() % (q - 1);
		auto r1 = solve(p, q, l);
		auto r2 = solveBF(p, q, l);
		bool diff = r1.size() != r2.size();
		if (!diff)
			for (auto& s : r2)
				if (r1.find(s) == r1.end()) {
					diff = true;
					break;
				}
		if (diff) {
			cout << p << "/" << q << ", pell: " << pell(p*q) << '\n';
			cout << "algo: " << '\n';
			for (auto& p : r1)
				cout << p << '\n';
			cout << "bf: " << '\n';
			for (auto& p : r2)
				cout << p << '\n';
			cout << "-------------" << '\n';
			f++;
		}
	}
	cout << f << " failed out of " << t << '\n';
}

auto compute() {
	ll b = 15, n = 21;
	ll lim = 1'000'000'000'000LL;

	while (n < lim)
		tie(b, n) = make_pair(3 * b + 2 * n - 2, 4 * b + 3 * n - 3);

	return b;
}

#ifdef _MSC_VER
	template <class T>
	inline void DoNotOptimize(const T &value) {
		__asm { lea ebx, value }
	}
#else
	template <class T>
	__attribute__((always_inline)) inline void DoNotOptimize(const T &value) {
		asm volatile("" : "+m"(const_cast<T &>(value)));
	}
#endif

int main() {
	using namespace std;
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	DoNotOptimize(result);
	cout << "Done in "
		<< duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6
		<< " miliseconds." << endl;
	cout << result << endl;
}