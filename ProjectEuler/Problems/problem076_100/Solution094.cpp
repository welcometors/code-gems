// https://projecteuler.net/problem=94
/*
Problem 94

Almost equilateral triangles

It is easily proved that no equilateral triangle exists with integral length
sides and integral area. However, the almost equilateral triangle 5-5-6 has
an area of 12 square units.

We shall define an almost equilateral triangle to be a triangle for which two
sides are equal and the third differs by no more than one unit.

Find the sum of the perimeters of all almost equilateral triangles with integral
side lengths and area and whose perimeters do not exceed one billion
(1,000,000,000).

Solution:

Let this be an almost equilateral triangle:

       +
      /|\
     / | \
    /  |  \
  a/   |   \a
  /   h|    \
 /     |     \
+------+------+
       b

a^2 = (b/2)^2 + h^2
Let b = a+1
Now,
a^2 = ((a+1)/2)^2 + h^2
3.a^2 - 2.a - 1 = 4.h^2
Multiplying by 3
9.a^2 - 6.a - 3 = 12.h^2
9.a^2 - 6.a + 1 = 12.h^2 + 4
(3.a - 1)^2 = 4(3.h^2 + 1)
((3.a - 1/2))^2 - 3.h^2 = 1
Let x = (3a - 1)/2, y = h and n = 3
x^2 - n.y^2 = 1	(Pell's equation)

Similarily, we can solve for b = a-1,
and get same equation for x = (3a + 1)/2, y = h and n = 3

Solving Pell's equation
for (x_k) + (y_k).sqrt(n) = (x_1 + y_1.sqrt(n))^k
for n = 3 first solution is 2,1
Now,
x_k+1 = x_1.x_k + n.y_1.y_k
y_k+1 = x_1.y_k + y_1.x_k

After getting x_k,y_k from Pell's equation, we can compute
for b = a-1
a = (2x-1)/3, b = 2(x-2)/3
parameter = 2x-2, area = y(x-2)/3

and, for b = a+1
a = (2x+1)/3, b = 2(x+2)/3
parameter = 2x+2, area = y(x+2)/3
*/

#include <iostream>
#include <tuple>
#include <chrono>
using namespace std;
using natural = unsigned long;

auto calc(natural limit) {
	natural x = 2, y = 1, p = 0;
	do {
		tie(x, y) = make_pair(2 * x + y * 3, 2 * y + x);
		natural a3 = 2 * x - 1; // area*3 = y*(x - 2);
		if (a3 - 1 >= limit)
			break;
		if (a3 % 3 == 0 && (y % 3 == 0 || (x - 2) % 3 == 0))
			p += a3 - 1;

		a3 += 2;
		// a3 = 2 * x + 1, area*3 = y*(x + 2);
		if (a3 + 1 >= limit)
			break;
		if (a3 % 3 == 0 && (y % 3 == 0 || (x + 2) % 3 == 0))
			p += a3 + 1;
	} while (true);
	return p;
}

auto compute() {
	const natural limit = 1'000'000'000;
	return calc(limit);
}

template<typename Function, class ... Types>
decltype(auto) timeit(Function f, Types ... args) {
    using namespace chrono;
    auto start = high_resolution_clock::now();
    auto result = f(args...);
    double duration = duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6;
    return std::make_pair(result, duration);
}

int main() {
    using namespace std;
    auto[result, time] = timeit(compute);
    cout << result << " Calculated in " << time << " miliseconds." << '\n';
    return 0;
}