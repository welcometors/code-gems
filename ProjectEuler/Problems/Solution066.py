import math

def solve(n):
	sqrtn = int(math.sqrt(n))
	if sqrtn*sqrtn == n:
		return 0

	f,b,a = 0, 1, sqrtn
	num_1,den_1 = 1,0
	num0,den0 = a,1
	while (num0*num0 - den0*den0*n) != 1:
		f = b*a - f
		b = (n - f*f) // b
		a = (sqrtn + f) // b

		tmp = num_1
		num_1 = num0
		num0 = a*num_1 + tmp
		tmp = den_1
		den_1 = den0
		den0 = a*den_1 + tmp
	return num0

max,maxd = 0,0
for d in range(2,1001):
    val = solve(d)
    if max < val:
        max,maxd = val,d

print("{}: {}".format(max,maxd))