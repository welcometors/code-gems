// https://projecteuler.net/problem=25
/*
1000-digit Fibonacci number

The Fibonacci sequence is defined by the recurrence relation:

Fn = Fn-1 + Fn-2, where F1 = 1 and F2 = 1.
Hence the first 12 terms will be:

F1 = 1
F2 = 1
F3 = 2
F4 = 3
F5 = 5
F6 = 8
F7 = 13
F8 = 21
F9 = 34
F10 = 55
F11 = 89
F12 = 144
The 12th term, F12, is the first term to contain three digits.

What is the index of the first term in the Fibonacci sequence to contain 1000 digits?

Solution:
Let phi = (1+sqrt(5))/2 i.e. golden-ratio
Let psi = (1-sqrt(5))/2

Fn = (phi^n - psi^n)/(phi - psi)
Fn = (phi^n - psi^n)/sqrt(5)
Since abs(psi^n/sqrt(5)) < 1/2 .... eq 1
Fn = [phi^n/sqrt(5)] rounded to nearest integer
10^1000 = phi^n/sqrt(5)
log(10^1000) = log(phi^n/sqrt(5))
1000 = n.log(phi) - log(sqrt(5))
n = (1000 + log(sqrt(5))) / log(phi)
it'll be slightly lower than that because of eq 1
So, n = ceil(1000 - 1 + log(sqrt(5))) / log(phi)
*/

#include <iostream>
#include <cmath>
using namespace std;

int main() {
	cout << ceil((1000 - 1 + log10(sqrt(5))) / log10((1 + sqrt(5)) / 2)) << endl;
}