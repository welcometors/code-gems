template <typename T>
bool isPalindrome(T n){
	T r = 0, c = n;
	while (c){
		r = r * 10 + c % 10;
		c /= 10;
	}
	return r == n;
}