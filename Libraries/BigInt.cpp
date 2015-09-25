#include <vector>
#include <string>
#include <iostream>
using namespace std;

class BigInt{
private:
	bool isPositive;
	vector<char> digits;

	static bool isLessThan(const BigInt &a, const BigInt &b){
		auto aLen = a.digits.size();
		auto bLen = b.digits.size();

		if (aLen != bLen)
			return aLen < bLen;

		for (int i = aLen - 1; i >= 0; i--)
			if (a.digits[i] != b.digits[i])
				return a.digits[i] < b.digits[i];

		return false;
	}
	static bool isGreaterOrEqual(const BigInt &a, const BigInt &b){
		return !isLessThan(a, b);
	}

	static bool isGreaterThan(const BigInt &a, const BigInt &b){
		auto aLen = a.digits.size();
		auto bLen = b.digits.size();

		if (aLen != bLen)
			return aLen > bLen;

		for (int i = aLen - 1; i >= 0; i--)
			if (a.digits[i] != b.digits[i])
				return a.digits[i] > b.digits[i];

		return false;
	}
	static bool isLessOrEqual(const BigInt &a, const BigInt &b){
		return !isGreaterThan(a, b);
	}

	static BigInt add(const BigInt &a, const BigInt &b){
		BigInt c;
		c.digits.clear();

		auto aLen = a.digits.size();
		auto bLen = b.digits.size();
		int ai = 0, bi = 0, cry = 0;
		while (ai < aLen && bi < bLen){
			auto res = a.digits[ai++] + b.digits[bi++] + cry;
			c.digits.push_back(res > 9 ? res - 10 : res);
			cry = res / 10;
		}

		while (ai < aLen){
			auto res = a.digits[ai++] + cry;
			c.digits.push_back(res > 9 ? res - 10 : res);
			cry = res / 10;
		}

		while (bi < bLen){
			auto res = b.digits[bi++] + cry;
			c.digits.push_back(res > 9 ? res - 10 : res);
			cry = res / 10;
		}

		if (cry)
			c.digits.push_back(cry);

		return c;
	}

	static BigInt sub(const BigInt &a, const BigInt &b){ // given a >= b
		BigInt c;
		c.digits.clear();

		auto aLen = a.digits.size();
		auto bLen = b.digits.size();
		int ai = 0, bi = 0, bro = 0;
		while (ai < aLen && bi < bLen){
			auto res = a.digits[ai++] - bro - b.digits[bi++];
			if (res < 0){
				c.digits.push_back(res + 10);
				bro = 1;
			}
			else{
				c.digits.push_back(res);
				bro = 0;
			}
		}

		while (ai < aLen){
			auto res = a.digits[ai++] - bro;
			if (res < 0){
				c.digits.push_back(res + 10);
				bro = 1;
			}
			else{
				c.digits.push_back(res);
				bro = 0;
			}
		}

		int li = aLen - 1;
		while (li > 0 && !c.digits[li])
			li--;

		if (li != aLen)
			c.digits.resize(li + 1);

		return c;
	}

	static BigInt mul(const BigInt &a, int d, int pow10multipler = 0){
		BigInt c;
		c.digits.clear();
		for (int i = 0; i < pow10multipler; i++)
			c.digits.push_back(0);

		int cry = 0;
		for (int i = 0, n = a.digits.size(); i < n; i++){
			int res = a.digits[i] * d + cry;
			c.digits.push_back(res % 10);
			cry = res / 10;
		}

		if (cry)
			c.digits.push_back(cry);

		return c;
	}

	static BigInt div(const BigInt& a, const BigInt& b, BigInt& remainder){
		if (b.isZero())
			throw std::overflow_error("Divide by zero");

		remainder = a;
		remainder.isPositive = true;

		if (!a.isZero() && isGreaterOrEqual(a, b)){
			auto aLen = a.digits.size();
			auto bLen = b.digits.size();
			int diff = aLen - bLen;
			BigInt quotient;
			quotient.digits.clear();

			do{
				BigInt multiplier, subtractor;
				multiplier.digits.clear();
				for (int i = 0; i < diff; i++)
					multiplier.digits.push_back(0);
				for (int i = 0; i < bLen; i++)
					multiplier.digits.push_back(b.digits[i]);

				int lesserIndex = 0;
				if (multiplier <= remainder){
					subtractor = multiplier;
					lesserIndex++;
					while (subtractor + multiplier <= remainder){
						subtractor = subtractor + multiplier;
						lesserIndex++;
					}
				}

				remainder = remainder - subtractor;
				if (quotient.digits.size() != 0 || lesserIndex != 0)
					quotient.digits.push_back(lesserIndex);
				diff--;
			} while (isGreaterOrEqual(remainder, b));

			while (diff > -1){
				quotient.digits.push_back(0);
				diff--;
			}

			std::reverse(quotient.digits.begin(), quotient.digits.end());
			return quotient;
		}

		return BigInt();
	}

public:
	BigInt() : digits({ char(0) }), isPositive(true){}

	BigInt(int val){
		isPositive = val >= 0;
		if (!val){
			digits.push_back(0);
			return;
		}

		if (val < 0) val = -val;
		while (val){
			digits.push_back(val % 10);
			val /= 10;
		}
	}

	BigInt(const BigInt& o) : digits(o.digits), isPositive(o.isPositive) {}

	BigInt(BigInt&& o) : digits(std::move(o.digits)), isPositive(o.isPositive) {}

	BigInt& operator= (const BigInt &o){ isPositive = o.isPositive; digits = o.digits; return *this; }

	bool isZero() const{
		return digits.size() == 1 && digits[0] == 0;
	}

	bool isOne() const{
		return digits.size() == 1 && digits[0] == 1;
	}

	string toString() const{
		string s = isPositive ? "" : "-";
		for (int i = digits.size() - 1; i >= 0; i--)
			s += (char)('0' + digits[i]);
		return s;
	}

	BigInt operator - () const {
		BigInt c = *this;
		if (!c.isZero())
			c.isPositive = !c.isPositive;
		return c;
	}

	friend bool operator < (const BigInt &a, const BigInt &b){
		if (a.isPositive != b.isPositive)
			return b.isPositive;

		if (a.isPositive)
			return isLessThan(a, b);

		return isGreaterThan(a, b);
	}

	friend bool operator >= (const BigInt &a, const BigInt &b){
		return !(a < b);
	}

	friend bool operator == (const BigInt &a, const BigInt &b){
		if (a.isPositive != b.isPositive)
			return false;

		auto aLen = a.digits.size();
		auto bLen = b.digits.size();

		if (aLen != bLen)
			return false;

		for (int i = aLen - 1; i >= 0; i--)
			if (a.digits[i] != b.digits[i])
				return false;

		return true;
	}

	friend bool operator != (const BigInt &a, const BigInt &b){
		return !(a == b);
	}

	friend bool operator > (const BigInt &a, const BigInt &b){
		if (a.isPositive != b.isPositive)
			return a.isPositive;

		if (a.isPositive)
			return isGreaterThan(a, b);

		return isLessThan(a, b);
	}

	friend bool operator <= (const BigInt &a, const BigInt &b){
		return !(a > b);
	}

	friend BigInt operator + (const BigInt &a, const BigInt &b){
		BigInt c;
		if (a.isPositive == b.isPositive){
			c = add(a, b);
			c.isPositive = a.isPositive;
		}
		else if (isGreaterThan(a, b)){
			c = sub(a, b);
			c.isPositive = a.isPositive;
		}
		else if (isLessThan(a, b)){
			c = sub(b, a);
			c.isPositive = b.isPositive;
		}
		return c;
	}

	friend BigInt operator - (const BigInt &a, const BigInt &b){
		return a + (-b);
	}

	friend BigInt operator * (const BigInt &a, const BigInt &b){
		BigInt sum;
		if (!a.isZero() && !b.isZero()){
			if (b.isOne())
				sum = a;
			else{
				for (int i = 0, n = b.digits.size(); i < n; i++){
					sum = sum + mul(a, b.digits[i], i);
				}
			}
			sum.isPositive = !(a.isPositive ^ b.isPositive);
		}
		return sum;
	}

	friend BigInt operator / (const BigInt &a, const BigInt &b){
		BigInt remainder;
		BigInt quotient = div(a, b, remainder);
		if (!quotient.isZero())
			quotient.isPositive = !(a.isPositive ^ b.isPositive);
		return quotient;
	}

	friend BigInt operator % (const BigInt &a, const BigInt &b){
		BigInt remainder;
		div(a, b, remainder);
		remainder.isPositive = a.isPositive;
		return remainder;
	}

	friend ostream &operator << (ostream &output, const BigInt &x){
		output << x.toString();
		return output;
	}
};

BigInt pow(const BigInt& x, int y){
	BigInt p = 1, cp = x;
	while (y){
		if (y & 1)
			p = cp * p;
		cp = cp * cp;
		y >>= 1;
	}
	return p;
}

void testLeassThan(){
	int a = 1234, b = 1254, c = -1234, d = -1254, e = 123, f = -123;
	BigInt ba = a, bb = b, bc = c, bd = d, be = e, bf = f;
#define BigCmpABTest(a, b, ba, bb) if ((a < b) != (ba < bb)) cout << a << " < " << b << endl
#define BigCmpABTest6( x, bx) \
	BigCmpABTest(x, a, bx, ba);	\
	BigCmpABTest(x, b, bx, bb);	\
	BigCmpABTest(x, c, bx, bc);	\
	BigCmpABTest(x, d, bx, bd);	\
	BigCmpABTest(x, e, bx, be);	\
	BigCmpABTest(x, f, bx, bf)

	BigCmpABTest6(a, ba);
	BigCmpABTest6(b, bb);
	BigCmpABTest6(c, bc);
	BigCmpABTest6(d, bd);
	BigCmpABTest6(e, be);
	BigCmpABTest6(f, bf);

#undef BigCmpABTest6
#undef BigCmpABTest
}

void testGreaterThan(){
	int a = 1234, b = 1254, c = -1234, d = -1254, e = 123, f = -123;
	BigInt ba = a, bb = b, bc = c, bd = d, be = e, bf = f;
#define BigCmpABTest(a, b, ba, bb) if ((a > b) != (ba > bb)) cout << a << " > " << b << endl
#define BigCmpABTest6( x, bx) \
	BigCmpABTest(x, a, bx, ba);	\
	BigCmpABTest(x, b, bx, bb);	\
	BigCmpABTest(x, c, bx, bc);	\
	BigCmpABTest(x, d, bx, bd);	\
	BigCmpABTest(x, e, bx, be);	\
	BigCmpABTest(x, f, bx, bf)

	BigCmpABTest6(a, ba);
	BigCmpABTest6(b, bb);
	BigCmpABTest6(c, bc);
	BigCmpABTest6(d, bd);
	BigCmpABTest6(e, be);
	BigCmpABTest6(f, bf);

#undef BigCmpABTest6
#undef BigCmpABTest
}

void testEquals(){
	int a = 1234, b = 1234, c = -1234, d = -1234, e = 0, f = 0;
	BigInt ba = a, bb = b, bc = c, bd = d, be = e, bf = f;
#define BigCmpABTest(a, b, ba, bb) if ((a == b) != (ba == bb)) cout << a << " == " << b << endl
#define BigCmpABTest6( x, bx) \
	BigCmpABTest(x, a, bx, ba);	\
	BigCmpABTest(x, b, bx, bb);	\
	BigCmpABTest(x, c, bx, bc);	\
	BigCmpABTest(x, d, bx, bd);	\
	BigCmpABTest(x, e, bx, be);	\
	BigCmpABTest(x, f, bx, bf)

	BigCmpABTest6(a, ba);
	BigCmpABTest6(b, bb);
	BigCmpABTest6(c, bc);
	BigCmpABTest6(d, bd);
	BigCmpABTest6(e, be);
	BigCmpABTest6(f, bf);

#undef BigCmpABTest6
#undef BigCmpABTest
}

void testPlus(){
	int a = 1234, b = 1254, c = -1234, d = -1254, e = 123, f = -123;
	BigInt ba = a, bb = b, bc = c, bd = d, be = e, bf = f;
#define BigCmpABTest(a, b, ba, bb) if ((a + b) != (ba + bb)) cout << a << " + " << b << endl
#define BigCmpABTest6( x, bx) \
	BigCmpABTest(x, a, bx, ba);	\
	BigCmpABTest(x, b, bx, bb);	\
	BigCmpABTest(x, c, bx, bc);	\
	BigCmpABTest(x, d, bx, bd);	\
	BigCmpABTest(x, e, bx, be);	\
	BigCmpABTest(x, f, bx, bf)

	BigCmpABTest6(a, ba);
	BigCmpABTest6(b, bb);
	BigCmpABTest6(c, bc);
	BigCmpABTest6(d, bd);
	BigCmpABTest6(e, be);
	BigCmpABTest6(f, bf);

#undef BigCmpABTest6
#undef BigCmpABTest
#undef BigCmpABTest
}

void testMinus(){
	int a = 1234, b = 1254, c = -1234, d = -1254, e = 123, f = -123;
	BigInt ba = a, bb = b, bc = c, bd = d, be = e, bf = f;
#define BigCmpABTest(a, b, ba, bb) if ((a - b) != (ba - bb)) cout << a << " - " << b << endl
#define BigCmpABTest6( x, bx) \
	BigCmpABTest(x, a, bx, ba);	\
	BigCmpABTest(x, b, bx, bb);	\
	BigCmpABTest(x, c, bx, bc);	\
	BigCmpABTest(x, d, bx, bd);	\
	BigCmpABTest(x, e, bx, be);	\
	BigCmpABTest(x, f, bx, bf)

	BigCmpABTest6(a, ba);
	BigCmpABTest6(b, bb);
	BigCmpABTest6(c, bc);
	BigCmpABTest6(d, bd);
	BigCmpABTest6(e, be);
	BigCmpABTest6(f, bf);

#undef BigCmpABTest6
#undef BigCmpABTest
#undef BigCmpABTest
}

void testMultiply(){
	int a = 123, b = 153, c = -123, d = -153, e = 12, f = -12;
	BigInt ba = a, bb = b, bc = c, bd = d, be = e, bf = f;
#define BigCmpABTest(a, b, ba, bb) if ((a * b) != (ba * bb)) cout << a << " * " << b << endl
#define BigCmpABTest6( x, bx) \
	BigCmpABTest(x, a, bx, ba);	\
	BigCmpABTest(x, b, bx, bb);	\
	BigCmpABTest(x, c, bx, bc);	\
	BigCmpABTest(x, d, bx, bd);	\
	BigCmpABTest(x, e, bx, be);	\
	BigCmpABTest(x, f, bx, bf)

	BigCmpABTest6(a, ba);
	BigCmpABTest6(b, bb);
	BigCmpABTest6(c, bc);
	BigCmpABTest6(d, bd);
	BigCmpABTest6(e, be);
	BigCmpABTest6(f, bf);

#undef BigCmpABTest6
#undef BigCmpABTest
#undef BigCmpABTest
}

void unitTests(){
	testLeassThan();
	testGreaterThan();
	testEquals();
	testPlus();
	testMinus();
	testMultiply();
}

void stressTests(int t, int lim){
	int failed = 0;
	for (int i = 0; i<t; i++){
		int a = -lim + (rand() % (2 * lim + 1)), b = -lim + (rand() % (2 * lim + 1));
		if (b == 0)
			b = 1;
		BigInt ba(a), bb(b);
		//cout << "Trying to divide" << a << "/" << b << "\n";
		if ((a % b) != (ba % bb)){
			cout << "Failed at " << a << "%" << b << " = " << (a % b) << " (expected), " << (ba % bb) << " (actual).\n";
			failed++;
		}
	}
	cout << "stressTests: " << t - failed << " is passed out of " << t << " (" << 100.0*(t - failed) / t << "% passed).\n";
}

int main(){
	//unitTests();
	stressTests(10000, 99999);
	//BigInt x;

	/*cout << (BigInt(123) + BigInt(123)) << endl;
	cout << 123 + 123 << endl;
	cout << ((BigInt(123) + BigInt(123)) == BigInt(123 + 123)) << endl;
	cout << x << endl;*/
	//cout << pow(BigInt(2), 30000) << endl;
	//cout << BigInt(-5816) / BigInt(286) << endl;
	//cout << BigInt(6949) % BigInt(12) << endl;*/
	system("pause");
}
/*
Failed at -5816/286 = -20 (expected), -2 (actual).
*/