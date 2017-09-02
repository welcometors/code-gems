// https://projecteuler.net/problem=84
/*
Problem 84

In the game, Monopoly, the standard board is set up in the following way:

GO	A1	CC1	A2	T1	R1	B1	CH1	B2	B3	JAIL
H2	 									C1
T2	 									U1
H1	 									C2
CH3	 									C3
R4	 									R2
G3	 									D1
CC3	 									CC2
G2	 									D2
G1	 									D3
G2J	F3	U2	F2	F1	R3	E3	E2	CH2	E1	FP


A player starts on the GO square and adds the scores on two 6-sided dice to 
determine the number of squares they advance in a clockwise direction. 
Without any further rules we would expect to visit each square with equal 
probability: 2.5%. However, landing on G2J (Go To Jail), CC (community chest), 
and CH (chance) changes this distribution.

In addition to G2J, and one card from each of CC and CH, that orders the 
player to go directly to jail, if a player rolls three consecutive doubles, 
they do not advance the result of their 3rd roll. Instead they proceed 
directly to jail.

At the beginning of the game, the CC and CH cards are shuffled. When a player 
lands on CC or CH they take a card from the top of the respective pile and, 
after following the instructions, it is returned to the bottom of the pile. 
There are sixteen cards in each pile, but for the purpose of this problem we 
are only concerned with cards that order a movement; any instruction not concerned 
with movement will be ignored and the player will remain on the CC/CH square.

Community Chest (2/16 cards):
	Advance to GO
	Go to JAIL
Chance (10/16 cards):
	Advance to GO
	Go to JAIL
	Go to C1
	Go to E3
	Go to H2
	Go to R1
	Go to next R (railway company)
	Go to next R
	Go to next U (utility company)
	Go back 3 squares.

The heart of this problem concerns the likelihood of visiting a particular square. 
That is, the probability of finishing at that square after a roll. For this reason 
it should be clear that, with the exception of G2J for which the probability of 
finishing on it is zero, the CH squares will have the lowest probabilities, as 5/8 
request a movement to another square, and it is the final square that the player 
finishes at on each roll that we are interested in. We shall make no distinction 
between "Just Visiting" and being sent to JAIL, and we shall also ignore the rule 
about requiring a double to "get out of jail", assuming that they pay to get out 
on their next turn.

By starting at GO and numbering the squares sequentially from 00 to 39 we can 
concatenate these two-digit numbers to produce strings that correspond with sets 
of squares.

Statistically it can be shown that the three most popular squares, in order, are 
JAIL (6.24%) = Square 10, E3 (3.18%) = Square 24, and GO (3.09%) = Square 00. So 
these three most popular squares can be listed with the six-digit modal 
string: 102400.

If, instead of using two 6-sided dice, two 4-sided dice are used, find the six-digit 
modal string.

Solution:

*/

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
using namespace std;

const size_t nstates = 40;
constexpr size_t Go = 0, Jail = 10, C1 = 11, E3 = 24, H2 = 39, R1 = 5, G2J = 30;

const vector<string> states = {
	"GO"  ,"A1","CC1","A2","T1","R1","B1" ,"CH1","B2","B3",
	"JAIL","C1","U1" ,"C2","C3","R2","D1" ,"CC2","D2","D3",
	"FP"  ,"E1","CH2","E2","E3","R3","F1" ,"F2" ,"U2","F3",
	"G2J" ,"G1","G2","CC3","G3","R4","CH3","H1" ,"T2","H2"
};

const vector<int> community = {
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0
};

const vector<int> chance = {
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0
};

const vector<size_t> nextR = { 15, 25, 5 };
const vector<size_t> nextU = { 12, 28, 12 };

using real = double;
using matrixRow = vector<real>;
using matrix = vector<matrixRow>;

auto getDiceProbabilities(int faces) {
	const real outcomes = faces*faces;
	vector<real> pTable(faces * 2 + 1);
	for (int i = 1; i <= faces; i++)
		pTable[1 + i] = pTable[2 * faces + 1 - i] = i / outcomes;
	return pTable;
}

auto getTransitionMatrix(int faces) {
	auto pTable = getDiceProbabilities(faces);
	matrix t(40, matrixRow(40, 0.0));

	for (size_t i = 0; i < nstates; i++) {
		for (size_t j = 2; j <= 2 * faces; j++) {
			real p = pTable[j];
			size_t i1 = (i + j) % nstates;
			if (chance[i1]) {
				p /= 16.0;
				t[i][i1] += 6.0 * p;
				t[i][Go] += p;
				t[i][Jail] += p;
				t[i][C1] += p;
				t[i][E3] += p;
				t[i][H2] += p;
				t[i][R1] += p;
				t[i][nextR[chance[i1] - 1]] += 2.0 * p;
				t[i][nextU[chance[i1] - 1]] += p;
				i1 = (i1 + nstates - 3) % nstates;
			}
			if (community[i1]) {
				p /= 16.0;
				t[i][i1] += 14.0 * p;
				t[i][Go] += p;
				t[i][Jail] += p;
			}
			else if(i1 == G2J)
				t[i][Jail] += p;
			else
				t[i][i1] += p;
		}
	}

	return t;
}

auto sqr(matrix &A) {
	// normalize so that every row sums to 1
	for (auto& r: A) {
		real s = 0;
		for (auto& e : r)
			s += e;
		for (auto& e : r)
			e /= s;
	}

	const size_t n = A.size();
	matrix B(n, matrixRow(n));
	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) {
			real v = 0;
			for (int i = 0; i < n; i++)
				v += A[r][i] * A[i][c];
			B[r][c] = v;
		}
	}

	return B;
}

auto compute() {
	auto t = getTransitionMatrix(4);
	for (int i = 0; i < 100; i++)
		t = sqr(t);

	vector<pair<real, size_t>> rank;
	for (int i = 0; i < nstates; i++)
		rank.push_back({ t[i][i], i });
		
	sort(rank.begin(), rank.end(), [](auto x, auto y) {return x.first > y.first;});

	/*for (auto& p : rank)
		cout << states[p.second] << '\t' << p.first << '\n';*/

	char out[7];
	snprintf(out, 7, "%02d%02d%02d", rank[0].second, rank[1].second, rank[2].second);
	return string(out);
}

template <class T>
inline void DoNotOptimize(const T &value) {
	__asm { lea ebx, value }
}

int main() {
	using namespace std;
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	DoNotOptimize(result);
	cout << "Done in "
		<< duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6
		<< " miliseconds." << endl;
	cout << result << endl;
}