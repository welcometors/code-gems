d = 1
n = 2

for i in range(2,101):
    c = 2*(i//3) if i%3 == 0 else 1
    t = d
    d = n
    n = c*d + t

print(sum([int(i) for i in str(n)]))