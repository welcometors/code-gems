#include <iostream>
#include <chrono>
#include <vector>
#include <iterator>
#include <limits>
#include <bitset>
#include <algorithm>
#include <cassert>

namespace mathLib {

	constexpr size_t compileTimeLogBase2(size_t n) {
		return (n <= 1) ? 0 : 1 + compileTimeLogBase2(n / 2);
	}

	using baseType = uint32_t;
	using baseTypeX2 = uint64_t;
	using vBaseType = std::vector<baseType>;
	constexpr size_t sizeOfBaseInBytes = sizeof(baseType);
	constexpr size_t baseBits = sizeOfBaseInBytes * 8;
	constexpr size_t lgBaseBits = compileTimeLogBase2(baseBits);
	constexpr baseTypeX2 baseLimit = std::numeric_limits<baseType>::max();
	constexpr baseTypeX2 baseLimitPlus1 = (baseTypeX2)(1) << baseBits;
	constexpr size_t minus1 = -1;
	constexpr double log2_base10 = 0.3010299956639812;

	// limits depend upon architechture
	constexpr size_t MulCacheBenefit = 16;
	constexpr size_t KaratsubaLimit = 70;
	constexpr size_t KaratsubaLimitSqr = 80;
	constexpr size_t ToomCookLimit = 216;
	constexpr size_t ToomCookLimitSqr = 216;
	constexpr size_t CommomDivLimit = 10;
	constexpr size_t CommomDivWorth = 8;
	constexpr size_t BurnikelZieglerLimit = 80;
	constexpr size_t SmallStringLimit = 8;

	const size_t log2TableFor32Bit[32] = {
		0,  9,  1, 10, 13, 21,  2, 29,
		11, 14, 16, 18, 22, 25,  3, 30,
		8, 12, 20, 28, 15, 17, 24,  7,
		19, 27, 23,  6, 26,  5,  4, 31 };

	size_t log2_32(uint32_t value) {
		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;
		return log2TableFor32Bit[(uint32_t)(value * 0x07C4ACDD) >> 27];
	}

	class integer {
	private:
		int sign;			// -1, 0, +1
		vBaseType digits;	// last (most-significant) will be non-zero

	public:

		// compare and return -1 (for less), 0 (for equal) and +1 (for greater)
		static int compare(const vBaseType &a, const vBaseType &b) {
			const auto len1 = a.size();
			const auto len2 = b.size();

			if (len1 < len2)
				return -1;
			else if (len1 > len2)
				return 1;

			for (size_t i = len2 - 1; i != minus1; --i)
				if (a[i] < b[i])
					return -1;
				else if (a[i] > b[i])
					return 1;

			return 0;
		}

		// shifts left by fixed amount of bits (< bitSize)
		inline static baseType shiftLeftShort(vBaseType &a, size_t bits) {
			if (bits) {
				baseTypeX2 carry = 0;
				for (auto& x : a) {
					carry |= (baseTypeX2(x) << bits);
					x = carry & baseLimit;
					carry >>= baseBits;
				}
				if (carry)
					a.push_back(baseType(carry));
				return baseType(carry);
			}
			return 0;
		}

		// shifts right by fixed amount of bits (< bitSize)
		inline static baseType shiftRightShort(vBaseType &a, size_t bits) {
			const size_t len = a.size();
			if (bits && len) {
				const baseType mask = (baseType(1) << bits) - 1;
				baseTypeX2 carry = 0;
				for (size_t i = len - 1; i != minus1; --i) {
					carry = (carry << baseBits) | a[i];
					a[i] = baseType(carry >> bits);
					carry &= mask;
				}
				if (!a.back())
					a.pop_back();
				return baseType(carry);
			}
			return 0;
		}

		// shifts left by any amount bits
		static vBaseType shiftLeft(const vBaseType &a, size_t bits) {
			vBaseType shifted;
			shifted.reserve(a.size() + (bits + baseBits - 1) / baseBits);

			if (bits >= baseBits) {
				shifted.resize(bits / baseBits, 0);
				bits &= baseBits - 1;
			}

			if (bits) {
				baseTypeX2 carry = 0;
				for (const auto x : a) {
					carry |= baseTypeX2(x) << bits;
					shifted.push_back(baseType(carry));
					carry >>= baseBits;
				}
				if (carry)
					shifted.push_back(baseType(carry));
			}
			else {
				std::copy(a.begin(), a.end(), std::back_inserter(shifted));
			}
			return shifted;
		}

		// shifts right by any amount bits
		static vBaseType shiftRight(const vBaseType &a, size_t bits) {
			const auto len = a.size();
			const size_t shift = bits / baseBits;
			if (shift >= len)
				return vBaseType();

			const size_t offset = bits & (baseBits - 1);
			if (!offset)
				return vBaseType(a.begin() + shift, a.end());

			vBaseType shifted;
			shifted.reserve(len - shift);
			const baseType cBits = baseBits - bits;

			for (size_t i = shift; i < len - 1; ++i)
				shifted.push_back((a[i + 1] << cBits) | (a[i] >> offset));

			const baseType last = a[len - 1] >> offset;
			if (last)
				shifted.push_back(last);

			return shifted;
		}

		// remove leading zeroes and corrects sign
		inline void normalize() {
			size_t i = digits.size() - 1;
			for (; i != minus1; --i)
				if (digits[i])
					break;
			if (i == minus1) {
				sign = 0;
				digits.clear();
			}
			else if (i + 1 < digits.size()) {
				digits.resize(i + 1);
			}
		}

		inline static size_t bitLength(const vBaseType& a) {
			return a.size() ? (a.size() - 1)*baseBits + log2_32(a.back()) + 1 : 0;
		}

		// a += b where b = b*2^(baseBits*pad)
		static void iAdd(vBaseType &a, const vBaseType &b, size_t pad = 0) {
			const auto len2 = b.size() + pad;
			if (a.size() < len2)
				a.resize(len2, 0);
			const auto len1 = a.size();

			baseTypeX2 carry = 0;
			size_t i = pad;
			for (; i < len2; ++i) {
				carry += a[i];
				carry += b[i - pad];
				a[i] = (baseType)carry;
				carry >>= baseBits;
			}
			for (; carry && i < len1; ++i) {
				carry += a[i];
				a[i] = (baseType)carry;
				carry >>= baseBits;
			}
			if (carry)
				a.push_back((baseType)carry);
		}

		// a -= b : given a >= b
		static baseType iSub(vBaseType &a, const vBaseType &b) {
			const auto len1 = a.size();
			const auto len2 = b.size();
			//assert(len1 >= len2);

			baseTypeX2 borrow = 0;
			size_t i = 0;
			for (;i < len2; ++i) {
				borrow = a[i] - borrow;
				borrow -= b[i];
				a[i] = (baseType)borrow;
				borrow >>= baseBits;
				borrow &= 1;
			}
			for (; borrow && i < len1; ++i) {
				borrow = a[i] - borrow;
				a[i] = (baseType)borrow;
				borrow >>= baseBits;
				borrow &= 1;
			}
			for (i = len1 - 1; i != minus1; --i)
				if (a[i])
					break;
			if (i != len1 - 1)
				a.resize(i + 1);
			return (baseType)borrow;
		}

		// a * b : given len(a) >= 2*len(b)
		static vBaseType mulBreaker(const vBaseType &a, const vBaseType &b,
			vBaseType(*mulFunction)(const vBaseType &a, const vBaseType &b)) {

			const auto len1 = a.size();
			const auto len2 = b.size();
			size_t last = ((len1 + len2 - 1) / len2 - 1) * len2;

			vBaseType tmp(a.begin() + last, a.end());
			tmp = mulFunction(b, tmp);
			vBaseType ans(tmp.size() + last, 0);
			std::copy(tmp.begin(), tmp.end(), ans.begin() + last);

			for (size_t i = 0; i < last; i += len2) {
				size_t iEnd = i + len2 - 1;
				while (iEnd != i - 1 && a[iEnd] == 0)
					--iEnd;
				if (iEnd != i - 1) {
					tmp.clear();
					std::copy(a.begin() + i, a.begin() + iEnd + 1, back_inserter(tmp));
					tmp = mulFunction(b, tmp);
					iAdd(ans, tmp, i);
				}
			}

			return ans;
		}

		// mulSimple requires N*M multiplications
		static vBaseType mulSimple(const vBaseType &a, const vBaseType &b) {
			const size_t len1 = a.size(), len2 = b.size();
			if (!len1 || !len2)
				return vBaseType();

			if (len1 + MulCacheBenefit < len2)
				return mulSimple(b, a);

			auto tmp = vBaseType(len1 + len2, 0);
			for (size_t i = 0; i < len2; i++) {
				baseTypeX2 carry = 0;
				baseTypeX2 multiplier = b[i];
				for (size_t j = 0; j < len1; j++) {
					carry = multiplier * a[j] + carry + tmp[i + j];
					tmp[i + j] = (baseType)carry;
					carry >>= baseBits;
				}
				tmp[i + len1] = (baseType)carry;
			}

			if (!tmp.back())
				tmp.pop_back();
			return tmp;
		}

		// sqrSimple requires N*N/2 multiplications
		static vBaseType sqrSimple(const vBaseType &a) {
			const size_t len = a.size();
			if (!len)
				return vBaseType();

			auto tmp = vBaseType(2 * len, 0);
			for (size_t i = 0; i < len; i++) {
				baseTypeX2 carry = baseTypeX2(a[i])*a[i] + tmp[i << 1];
				baseType nextCarry = 0;
				tmp[i << 1] = (baseType)carry;
				carry >>= baseBits;
				baseTypeX2 multiplier = baseTypeX2(a[i]) << 1;

				if (multiplier > baseLimit) {
					multiplier &= baseLimit;
					for (size_t j = i + 1; j < len; j++) {
						carry = multiplier * a[j] + carry + tmp[i + j];
						tmp[i + j] = (baseType)carry;
						carry = (carry >> baseBits) + nextCarry + a[j];
						nextCarry = carry >> baseBits;
						carry &= baseLimit;
					}
				}
				else {
					for (size_t j = i + 1; j < len; j++) {
						carry = multiplier * a[j] + carry + tmp[i + j];
						tmp[i + j] = (baseType)carry;
						carry >>= baseBits;
					}
				}

				carry += tmp[i + len];
				tmp[i + len] = (baseType)carry;
				nextCarry += carry >> baseBits;
				if (nextCarry)
					tmp[i + len + 1] = nextCarry;
			}

			if (!tmp.back())
				tmp.pop_back();
			return tmp;
		}

		// returns subArray of digits
		inline static vBaseType subValue(const vBaseType &a, size_t start, size_t end = minus1) {
			const size_t len = a.size();
			if (start < len) {
				size_t iEnd = std::min({ end, len - 1 });
				while (iEnd + 1 != start && a[iEnd] == 0)
					--iEnd;
				if (iEnd + 1 != start)
					return vBaseType(a.begin() + start, a.begin() + iEnd + 1);
			}
			return vBaseType();
		}

		// Below 3 functions implement Karatsuba multiplication algorithm
		inline static vBaseType joinKaratsuba(size_t mid, vBaseType &z2, vBaseType &z1, vBaseType &z0) {
			iSub(z1, z2);
			iSub(z1, z0);
			vBaseType ans(z2.size() + mid * 2, 0);
			std::copy(z0.begin(), z0.end(), ans.begin());
			iAdd(ans, z1, mid);
			iAdd(ans, z2, mid * 2);
			return ans;
		}

		static vBaseType mulKaratsuba(const vBaseType &a, const vBaseType &b) {
			const auto len1 = a.size();
			const auto len2 = b.size();

			if (len1 < KaratsubaLimit || len2 < KaratsubaLimit)
				return mulSimple(a, b);

			if (len1 >= 2 * len2)
				return mulBreaker(a, b, mulKaratsuba);
			else if (len2 >= 2 * len1)
				return mulBreaker(b, a, mulKaratsuba);

			size_t mid = (std::max(len1, len2) + 1) >> 1;
			auto ah = subValue(a, mid);
			auto al = subValue(a, 0, mid - 1);
			auto bh = subValue(b, mid);
			auto bl = subValue(b, 0, mid - 1);

			auto z0 = mulKaratsuba(al, bl);
			auto z2 = mulKaratsuba(ah, bh);
			iAdd(al, ah);
			iAdd(bl, bh);
			auto z1 = mulKaratsuba(al, bl);
			return joinKaratsuba(mid, z2, z1, z0);
		}

		static vBaseType sqrKaratsuba(const vBaseType &a) {
			const auto len = a.size();
			if (len < KaratsubaLimitSqr)
				return sqrSimple(a);

			size_t mid = (len + 1) >> 1;
			auto ah = subValue(a, mid);
			auto al = subValue(a, 0, mid - 1);
			auto z0 = sqrKaratsuba(al);
			auto z2 = sqrKaratsuba(ah);
			iAdd(al, ah);
			auto z1 = sqrKaratsuba(al);
			return joinKaratsuba(mid, z2, z1, z0);
		}

		// Below 4 functions implements ToomCook 3 split multiplication algorithm
		inline static void splitToomCook3(const vBaseType& a, size_t slice, integer &a0, integer &a1, integer &a2) {
			a0.digits = subValue(a, slice * 2);
			a0.sign = a0.digits.size() != 0;
			a1.digits = subValue(a, slice, slice * 2 - 1);
			a1.sign = a1.digits.size() != 0;
			a2.digits = subValue(a, 0, slice - 1);
			a2.sign = a2.digits.size() != 0;
		}

		inline static vBaseType joinToomCook3(size_t slice, integer &vi, integer &v2, integer &v1, integer &vm1, integer &v0) {
			v2 -= vm1;
			iDiv(v2.digits, 3);
			v2.sign = v2.digits.size() != 0;
			auto tm1 = v1 - vm1;
			shiftRightShort(tm1.digits, 1);
			tm1.sign = tm1.digits.size() != 0;
			v1 -= v0;
			v2 -= v1;
			shiftRightShort(v2.digits, 1);
			v2.sign = v2.digits.size() != 0;
			v1 -= tm1;
			v1 -= vi;
			v2 -= vi;
			v2 -= vi;
			tm1 -= v2;

			vBaseType ans(vi.digits.size() + slice * 4, 0);
			std::copy(v0.digits.begin(), v0.digits.end(), ans.begin());
			iAdd(ans, tm1.digits, slice);
			iAdd(ans, v1.digits, slice * 2);
			iAdd(ans, v2.digits, slice * 3);
			iAdd(ans, vi.digits, slice * 4);
			return ans;
		}

		static vBaseType mulToomCook3(const vBaseType &a, const vBaseType &b) {
			const auto len1 = a.size();
			const auto len2 = b.size();

			if (len1 < ToomCookLimit || len2 < ToomCookLimit)
				return mulKaratsuba(a, b);

			if (len1 >= 2 * len2)
				return mulBreaker(a, b, mulToomCook3);
			else if (len2 >= 2 * len1)
				return mulBreaker(b, a, mulToomCook3);

			size_t slice = (std::max(len1, len2) + 2) / 3;
			integer a0, a1, a2;
			splitToomCook3(a, slice, a2, a1, a0);
			integer b0, b1, b2;
			splitToomCook3(b, slice, b2, b1, b0);

			auto v0 = integer(mulToomCook3(a0.digits, b0.digits));
			auto da1 = a2 + a0;
			auto db1 = b2 + b0;
			auto da1_a1 = da1 - a1;
			auto db1_b1 = db1 - b1;
			auto vm1 = integer(mulToomCook3(da1_a1.digits, db1_b1.digits), da1_a1.sign*db1_b1.sign);
			da1 += a1;
			db1 += b1;
			auto v1 = integer(mulToomCook3(da1.digits, db1.digits), da1.sign*db1.sign);
			da1 += a2;
			shiftLeftShort(da1.digits, 1);
			da1 -= a0;
			db1 += b2;
			shiftLeftShort(db1.digits, 1);
			db1 -= b0;
			auto v2 = integer(mulToomCook3(da1.digits, db1.digits), da1.sign*db1.sign);
			auto vi = integer(mulToomCook3(a2.digits, b2.digits));
			return joinToomCook3(slice, vi, v2, v1, vm1, v0);
		}

		static vBaseType sqrToomCook3(const vBaseType &a) {
			const auto len = a.size();
			if (len < ToomCookLimitSqr)
				return sqrKaratsuba(a);

			size_t slice = (len + 2) / 3;
			integer a0, a1, a2;
			splitToomCook3(a, slice, a2, a1, a0);

			auto v0 = integer(sqrToomCook3(a0.digits));
			auto da1 = a2 + a0;
			auto da1_a1 = da1 - a1;
			auto vm1 = integer(sqrToomCook3(da1_a1.digits));
			da1 += a1;
			auto v1 = integer(sqrToomCook3(da1.digits));
			da1 += a2;
			shiftLeftShort(da1.digits, 1);
			da1 -= a0;
			auto v2 = integer(sqrToomCook3(da1.digits));
			auto vi = integer(sqrToomCook3(a2.digits));
			return joinToomCook3(slice, vi, v2, v1, vm1, v0);
		}

		/*
		static void appendBits(vBaseType &a, size_t aBitLength, const vBaseType &b, size_t bStart, size_t bBitLength) {
		size_t aIdx = a.size() - 1 - aBitLength / baseBits;
		size_t bit32 = aBitLength & (baseBits - 1);
		size_t bIdx = b.size() - 1 - bStart;
		const size_t bIdxStop = bIdx - bBitLength / baseBits;

		if (bit32) {
		for (; bIdx > bIdxStop; --bIdx) {
		a[aIdx] |= b[bIdx] << bit32;
		aIdx--;
		a[aIdx] = b[bIdx] >> (baseBits - bit32);
		}
		}
		else {
		std::copy(b.begin() + bIdxStop)
		for (; bIdx > bIdxStop; --bIdx) {
		a[aIdx] = b[bIdx];
		aIdx--;
		}
		}

		if (bBitLength % 32 > 0) {
		int bi = b[bIdx];
		bi &= -1 >> > (32 - bBitLength % 32);
		a[aIdx] |= bi << bit32;
		if (bit32 + (bBitLength % 32) > 32)
		a[aIdx - 1] = bi >> > (32 - bit32);
		}
		}

		static vBaseType mulSchoenhageStrassen(const vBaseType &a, const vBaseType &b) {
		const auto len1 = a.size();
		const auto len2 = b.size();

		bool isSqr = &a == &b;
		size_t m = log2_32(2 * std::max(len1, len2) * baseBits - 1 - 1);
		size_t n = (m >> 1) + 1;
		size_t numPieces = (m & 1) ? (1 << n) : (1 << (n + 1));
		size_t pieceSize = 1 << (n - 1 - 5);
		size_t numPiecesA = (len1 + pieceSize) / pieceSize;
		int[] u = new int[(numPiecesA*(3 * n + 5) + 31) / 32];
		}
		*/

		// returns a*b
		static vBaseType iMul(const vBaseType &a, const vBaseType &b) {
			const size_t len1 = a.size(), len2 = b.size();
			if (!len1 || !len2)
				return vBaseType();

			if (&a == &b) {
				if (len1 < KaratsubaLimitSqr)
					return sqrSimple(a);
				if (len1 < ToomCookLimitSqr)
					return sqrKaratsuba(a);
				return sqrToomCook3(a);
			}

			if (len1 < KaratsubaLimit || len2 < KaratsubaLimit)
				return mulSimple(a, b);
			if (len1 < ToomCookLimit || len2 < ToomCookLimit)
				return mulKaratsuba(a, b);
			return mulToomCook3(a, b);
		}

		// a /= b and returns a % b: given b <= baseLimit
		static baseType iDiv(vBaseType &a, baseType b) {
			const auto len = a.size();
			if (!len)
				return 0;

			if (len == 1) {
				baseType r = a[0];
				a[0] /= b;
				r -= a[0] * b;

				if (!a.back())
					a.pop_back();
				return (baseType)r;
			}

			baseTypeX2 r = 0;
			for (size_t i = len - 1; i != minus1; --i) {
				r = (r << baseBits) | a[i];
				a[i] = (baseType)(r / b);
				r -= baseTypeX2(a[i]) * b;
			}

			if (!a.back())
				a.pop_back();
			return (baseType)r;
		}

		// division algorithm D in Knuth section 4.3.1.
		static vBaseType divKnuth(const vBaseType &dividend, const vBaseType &divisor, vBaseType &quotient) {
			const size_t len2 = divisor.size();
			const size_t len1 = dividend.size();

			if (len2 >= CommomDivLimit) {
				size_t i = 0;
				while (i < len2 && dividend[i] == 0 && divisor[i] == 0)
					++i;
				if (i > CommomDivWorth) {
					vBaseType a(dividend.begin() + i, dividend.end());
					vBaseType b(divisor.begin() + i, divisor.end());
					return iDiv(a, b, quotient);
				}
			}

			size_t d = baseBits - 1 - log2_32(divisor.back());
			vBaseType v = vBaseType(divisor);
			shiftLeftShort(v, d);
			vBaseType u = vBaseType(dividend);
			if (!shiftLeftShort(u, d))
				u.push_back(0);

			const size_t qSize = len1 - len2;
			quotient.resize(qSize + 1);
			vBaseType rem(len2);
			baseTypeX2 vn_1 = v[len2 - 1], vn_2 = v[len2 - 2];

			for (size_t j = qSize; j != minus1; --j) {
				baseTypeX2 qval = (baseTypeX2(u[j + len2]) << baseBits) | u[j + len2 - 1];
				baseTypeX2 qhat = qval / vn_1;
				baseTypeX2 rhat = qval - qhat * vn_1;
				while (qhat*vn_2 > ((rhat << baseBits) | u[j + len2 - 2])) {
					--qhat;
					rhat += vn_1;
					if (rhat > baseLimit)
						break;
				}

				//ToDo: combine multiple and subtract in one without rem
				baseTypeX2 k = 0;
				for (size_t i = 0; i < len2; ++i) {
					k += qhat * v[i];
					rem[i] = (baseType)k;
					k >>= baseBits;
				}
				baseTypeX2 b = 0;
				for (size_t i = 0; i < len2; ++i) {
					b = u[i + j] - b;
					b -= rem[i];
					u[i + j] = (baseType)b;
					b = (b >> baseBits) & 1;
				}

				if (u[j + len2] < b) {
					--qhat;
					k = 0;
					for (size_t i = 0; i < len2; ++i) {
						k += baseTypeX2(u[i + j]) + v[i];
						u[i + j] = (baseType)k;
						k >>= baseBits;
					}
				}
				quotient[j] = (baseType)qhat;
			}
			while (quotient.size() && !quotient.back())
				quotient.pop_back();

			u[len2] = 0;
			if (d) {
				for (size_t i = 0; i < len2; ++i)
					rem[i] = (u[i] >> d) | (u[i + 1] << (baseBits - d));
			}
			else {
				std::copy(u.begin(), u.begin() + len2, rem.begin());
			}

			while (rem.size() && !rem.back())
				rem.pop_back();
			return rem;
		}

		// pad 0s to make it blockSize then append src
		static void concatBlock(vBaseType &dest, const vBaseType &src, size_t blockSize) {
			if (src.size()) {
				dest.resize(blockSize + src.size(), 0);
				std::copy(src.begin(), src.end(), dest.begin() + blockSize);
			}
		}

		// Below 3 functions implements Burnikel and Ziegler division algorithm
		static vBaseType divBurnikelZiegler(const vBaseType &dividend, const vBaseType &divisor, vBaseType &quotient) {
			const size_t len1 = dividend.size();
			const size_t len2 = divisor.size();

			const size_t m = 1 << (1 + log2_32(len2 / BurnikelZieglerLimit));
			const size_t n = ((len2 + m - 1) / m)*m;
			const size_t nBits = n*baseBits;
			const size_t sigma = nBits - bitLength(divisor);
			vBaseType b = shiftLeft(divisor, sigma);
			vBaseType a = shiftLeft(dividend, sigma);

			size_t t = (bitLength(a) + nBits) / nBits;
			if (t < 2) {
				std::cout << "<unreachable>";
				t = 2;
			}

			vBaseType qi, ri;
			quotient.clear();
			auto z = subValue(a, (t - 2)*n);

			for (size_t i = t - 2, in = i*n; i; i--, in -= n) {
				ri = divide2n1n(z, b, qi);
				z = subValue(a, in - n, in - 1);
				concatBlock(z, ri, n);
				iAdd(quotient, qi, in);
			}

			ri = divide2n1n(z, b, qi);
			iAdd(quotient, qi);
			return shiftRight(ri, sigma);
		}

		static vBaseType divide2n1n(const vBaseType &a, const vBaseType &b, vBaseType &q) {
			size_t n = b.size();
			if (n & 1 || n < BurnikelZieglerLimit)
				return iDiv(a, b, q);

			n /= 2;
			q.clear();
			if (a.size() <= 3 * n)
				return divide3n2n(a, b, q);

			const size_t nBits = n * baseBits;
			vBaseType q1;
			auto a123 = shiftRight(a, nBits);
			auto r1 = divide3n2n(a123, b, q1);
			auto a0 = subValue(a, 0, n - 1);
			concatBlock(a0, r1, n);
			auto r2 = divide3n2n(a0, b, q);
			concatBlock(q, q1, n);
			return r2;
		}

		static vBaseType divide3n2n(const vBaseType &a, const vBaseType &b, vBaseType &q) {
			const size_t n = b.size() / 2;
			const size_t nBits = n * baseBits;

			vBaseType a12 = shiftRight(a, nBits);
			vBaseType b1 = subValue(b, n, 2 * n - 1);
			vBaseType r = divide2n1n(a12, b1, q);
			vBaseType a3 = subValue(a, 0, n - 1);
			concatBlock(a3, r, n);
			r = std::move(a3);

			vBaseType qb2 = iMul(q, subValue(b, 0, n - 1));
			while (compare(r, qb2) < 0) {
				iAdd(r, b);
				iSub(q, vBaseType{ 1 });
			}
			iSub(r, qb2);
			return r;
		}

		// q = a / b and returns a % b
		static vBaseType iDiv(const vBaseType &a, const vBaseType &b, vBaseType &q) {
			const size_t len2 = b.size();
			if (len2 == 0)
				throw std::overflow_error("divide by integer(0).");

			const size_t len1 = a.size();
			int cmp = compare(a, b);
			if (cmp < 0) {
				vBaseType r = a;
				q = vBaseType();
				return r;
			}
			if (!cmp) {
				q = vBaseType{ 1 };
				return vBaseType();
			}

			if (len2 <= 1) {
				q = a;
				return vBaseType{ iDiv(q, b.back()) };
			}

			if (len2 < BurnikelZieglerLimit || len1 - len2 < BurnikelZieglerLimit)
				return divKnuth(a, b, q);

			return divBurnikelZiegler(a, b, q);
		}

		// returns square root
		integer sqrt() const {
			if (sign < 0)
				throw std::overflow_error("sqrt of negative integer.");

			if (sign == 0)
				return integer();

			if (digits.size() == 1)
				return integer(int(std::sqrt(digits.front())));

			vBaseType two = { 2 }, y = shiftLeft({ 1 }, (bitLength(digits) + 1) / 2), q;
			iSub(y, { 1 });
			iDiv(digits, y, q);
			while (compare(y, q) > 0) {
				iAdd(y, q);
				shiftRightShort(y, 1);
				iDiv(digits, y, q);
			}

			return integer(y);
		}

		integer() : sign(0) {}

		explicit integer(int val) {
			if (val) {
				if (val < 0) {
					sign = -1;
					val = -val;
				}
				else {
					sign = 1;
				}
				digits.push_back(val & baseLimit);
			}
			else {
				sign = 0;
			}
		}

		explicit integer(const vBaseType &values, int sign = 1)
			: digits(values), sign(sign) {
			normalize();
		}

		explicit integer(vBaseType &&values, int sign = 1)
			: digits(std::move(values)), sign(sign) {
			normalize();
		}

		integer(const integer &o)
			: digits(o.digits), sign(o.sign) {}

		integer(integer &&o)
			: digits(std::move(o.digits)), sign(o.sign) {}

		integer& operator= (const integer &o) {
			sign = o.sign;
			digits = o.digits;
			return *this;
		}

		integer& operator= (integer &&o) {
			sign = o.sign;
			digits = std::move(o.digits);
			return *this;
		}

		inline bool isZero() const {
			return sign == 0;
		}

		inline size_t size() const {
			return digits.size();
		}

		inline size_t bitLength() const {
			return sign ? (digits.size() - 1)*baseBits + log2_32(digits.back()) + 1 : 0;
		}

		std::string toBinaryString() const {
			if (!sign)
				return "0";

			std::string binary = sign > 0 ? "" : "-";
			binary += std::bitset<baseBits>(digits.back()).to_string();
			binary = binary.substr(binary.find('1'));
			binary.reserve(binary.size() + (digits.size() - 1)*baseBits + 1);

			for (size_t i = digits.size() - 2; i != minus1; --i)
				binary += std::bitset<baseBits>(digits[i]).to_string();
			return binary;
		}

		static std::vector<vBaseType> tenPow2PowNs;

		static vBaseType& tenPow2PowN(size_t n) {
			while (tenPow2PowNs.size() <= n) {
				auto sqr = sqrToomCook3(tenPow2PowNs.back());
				tenPow2PowNs.emplace_back(sqr);
			}
			return tenPow2PowNs[n];
		}

		static void toStringFill(std::string &decimal, vBaseType& num, size_t _10p2pN, size_t offset) {
			if (_10p2pN < SmallStringLimit) {
				while (num.size()) {
					auto rem = iDiv(num, 1'000'000'000);
					for (auto i = 0; rem && i < 9; i++) {
						baseType q = rem / 10;
						decimal[offset + i] = (rem - q * 10) + '0';
						rem = q;
					}
					offset += 9;
				}
				return;
			}

			auto pow10 = tenPow2PowN(_10p2pN);
			auto rem = iDiv(num, pow10, num);
			toStringFill(decimal, rem, _10p2pN - 1, offset);
			toStringFill(decimal, num, _10p2pN - 1, offset + (1 << _10p2pN));
		}

		std::string toString() const {
			size_t numDigits = ceil(digits.size() * baseBits * mathLib::log2_base10);
			std::string decimal(numDigits, '0');
			vBaseType val = digits;
			toStringFill(decimal, val, log2_32(numDigits), 0);

			while (decimal.back() == '0')
				decimal.pop_back();
			if (sign < 0)
				decimal += '-';

			reverse(decimal.begin(), decimal.end());
			return decimal;
		}

		integer& negate() {
			sign *= -1;
			return *this;
		}

		bool operator < (const integer &other) const {
			if (sign != other.sign)
				return sign < other.sign;
			return sign ? compare(digits, other.digits) == -sign : false;
		}

		inline bool operator >= (const integer &other) const {
			return !(*this < other);
		}

		bool operator == (const integer &other) const {
			return sign == other.sign ? !compare(digits, other.digits) : false;
		}

		inline bool operator != (const integer &other) const {
			return !(*this == other);
		}

		bool operator > (const integer &other) const {
			if (sign != other.sign)
				return sign > other.sign;
			return sign ? compare(digits, other.digits) == sign : false;
		}

		inline bool operator <= (const integer &other) const {
			return !(*this > other);
		}

		inline void addOrsubtract(const vBaseType& other, bool isAdd, int newSign) {
			if (!sign) {
				digits = other;
				sign = newSign;
			}
			else if (isAdd) {
				iAdd(digits, other);
			}
			else {
				int cmp = compare(digits, other);
				if (!cmp)
					digits.clear();
				else if (cmp > 0)
					iSub(digits, other);
				else {
					vBaseType copy(other);
					iSub(copy, digits);
					digits = std::move(copy);
					sign = newSign;
				}
				if (!digits.size())
					sign = 0;
			}
		}

		integer& operator += (const integer &other) {
			if (other.sign)
				addOrsubtract(other.digits, sign == other.sign, other.sign);
			return *this;
		}

		integer& operator -= (const integer &other) {
			if (other.sign)
				addOrsubtract(other.digits, sign != other.sign, -other.sign);
			return *this;
		}

		inline friend integer operator + (integer lhs, const integer &rhs) {
			lhs += rhs;
			return lhs;
		}

		inline friend integer operator - (integer lhs, const integer &rhs) {
			lhs -= rhs;
			return lhs;
		}

		inline integer operator * (const integer &other) const {
			if (!sign || !other.sign)
				return integer();

			auto product = iMul(digits, other.digits);
			return integer(product, sign*other.sign);
		}

		integer operator << (size_t bits) const {
			return integer(shiftLeft(digits, bits), sign);
		}

		integer operator >> (size_t bits) const {
			return integer(shiftRight(digits, bits), sign);
		}

		integer operator & (const integer &other) const {
			const auto len1 = digits.size();
			const auto len2 = other.digits.size();

			if (len1 == 1 && len2 == 1)
				return integer(vBaseType({ digits[0] & other.digits[0] }));

			size_t iEnd = std::min(len1, len2) - 1;
			while (iEnd && (digits[iEnd] & other.digits[iEnd]) == 0)
				--iEnd;

			vBaseType tmp;
			tmp.reserve(iEnd + 1);
			for (size_t i = 0; i <= iEnd; ++i)
				tmp.push_back(digits[i] & other.digits[i]);
			return integer(tmp);
		}

		integer operator | (const integer &other) const {
			const auto len1 = digits.size();
			const auto len2 = other.digits.size();

			if (len1 == 1 && len2 == 1)
				return integer(vBaseType({ digits[0] | other.digits[0] }));

			vBaseType tmp;
			tmp.reserve(std::max(len1, len2));

			const auto minLen = std::min(len1, len2);
			size_t i = 0;
			for (; i < minLen; ++i)
				tmp.push_back(digits[i] | other.digits[i]);

			copy(digits.begin() + i, digits.end(), back_inserter(tmp));
			copy(other.digits.begin() + i, other.digits.end(), back_inserter(tmp));
			return integer(tmp);
		}

		friend std::ostream &operator << (std::ostream &output, const integer &x) {
			output << x.toString();
			return output;
		}
	};

	std::vector<vBaseType> integer::tenPow2PowNs = { { 10 } };
};