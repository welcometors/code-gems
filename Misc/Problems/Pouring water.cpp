// default

#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

#define natural unsigned int
#ifdef _MSC_VER
#define readNatural(n) scanf_s("%u", &n)
#else
#define readNatural(n) scanf("%u", &n)
#endif
#define writeNatural(n) printf("%u\n", n)

natural gcd(natural a, natural b){
	natural t;
	while (b){
		t = a;
		a = b;
		b = t % b;
	}
	return a;
}

int main(){
	natural t, a, b, c, m1, m2, k;
	readNatural(t);

	while (t--){
		readNatural(a);
		readNatural(b);
		readNatural(c);

		if (c == a || c == b){
			cout << "1" << endl;
			continue;
		}

		if (a > b){
			a = a ^ b;
			b = a ^ b;
			a = a ^ b;
		}

		if (c > b || (c % gcd(b, a)) != 0){
			cout << "-1" << endl;
			continue;
		}

		m1 = k = 0;
		while (k != c){
			natural d = c - k;
			if ( c > k && d % a == 0){	// either reach to c
				m1 += 2 * (d / a);		// filled and poured d/a times
				k = c;
			}
			else{
				d = ((b - k) / a) + 1;	// or reach to >= b
				m1 += 2 * d;			// filled and poured d times
				k += a * d;
			}
			if (k != c && k > b){
				k -= b;
				if (k == c)
					break;
				m1++;					// empty b
				m1++;					// pouring a in b
			}
		}

		m2 = k = 0;
		while (k != c){
			k += b;				
			m2++;						// filling b
			natural d = k - c;
			if (k > a && d % a == 0){	// either reach to c
				m2 += 2 * (d / a) - 1;	// filled and poured d/a times
				k = c;
			}
			else{
				d = ((k - a) / a) + 1;	// or reach to <= a
				m2 += 2 * d;			// filled and poured d times
				k -= a*d;
			}
			if (k != c)
				m2++;					// pouring b in a
		}

		cout << ((m1 < m2) ? (m1) : (m2)) << endl;
	}

#ifdef _MSC_VER
	std::system("pause");
#endif
}

// 4 5 2 3 2 3 4 3 5 4 3 7 5
// 2 -1 6 8

/*
OLD CODE

m1 = k = 0;
while (k != c){
	while (k != c && k <= b){
		m1++;			// filling a
		k += a;
		m1++;			// pouring a in b
	}
	if (k != c && k > b){
		k -= b;
		if (k == c)		// a has result
			break;
		m1++;			// empty b
		m1++;			// pouring a in b
	}
}

m2 = k = 0;
while (k != c){
	k += b;				// filling b
	m2++;
	while (k != c && k > a){
		k -= a;
		m2++;			// pouring b in a
		if (k == c)
			break;
		m2++;			// empty a
	}
	if (k != c)
		m2++;			// pouring b in a
}
*/