import math
from decimal import *

getcontext().prec = 200
getcontext().rounding = ROUND_DOWN

def get_decimal_sum(n):
	s = str(Decimal(n).sqrt())
	#print(s)
	i = s.index('.')
	s = s[:i] + s[i+1:]
	#print(s)
	return sum([int(c) for c in s[:100]])

#print(get_decimal_sum(99))

def main():
	d = 0
	for i in range(2,101):
		j = int(math.sqrt(i))
		if j*j != i:
			d += get_decimal_sum(i)
			#print(get_decimal_sum(i))
			#break
	print(d)

if __name__ == '__main__':
	main()