template <typename T>
bool isPalindrome(T n){
	T r = 0, c = n;
	while (c){
		r = r * 10 + c % 10;
		c /= 10;
	}
	return r == n;
}

unsigned long isqrt(unsigned long x){
	register unsigned long op, res, one;
	op = x;
	res = 0;
	/* "one" starts at the highest power of four <= than the argument. */
	one = 1 << 30;  /* second-to-top bit set */
	while (one > op)
		one >>= 2;

	while (one != 0){
		if (op >= res + one){
			op -= res + one;
			res += one << 1;
		}
		res >>= 1;
		one >>= 2;
	}
	return res;
}