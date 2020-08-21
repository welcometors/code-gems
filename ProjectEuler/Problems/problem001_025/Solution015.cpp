// https://projecteuler.net/problem=15
/*
Lattice paths

Starting in the top left corner of a 2×2 grid, and only being able to move to the right and down, 
there are exactly 6 routes to the bottom right corner.

How many such routes are there through a 20×20 grid?

Solution:
In a grid of 2x2 there will always be 2 horizontal lines and 2 vertical lines.
which will make patterns like
HHVV, HVHV, HVVH, VVHH, VHVH, VHHV
This is same as arrenging 2 V and 2 H. Which is 2C2 = 6
*/

#include <iostream>
using namespace std;
using natural = uint64_t;

int main() {
	natural ncr = 40;
	
	for (int i = 39, j = 2; j <= 20; i--, j++)
		ncr = (ncr / j)*i;

	cout << ncr << '\n';
}