// https://projecteuler.net/problem=54
/*
Poker hands

In the card game poker, a hand consists of five cards and are ranked,
from lowest to highest, in the following way:

High Card: Highest value card.
One Pair: Two cards of the same value.
Two Pairs: Two different pairs.
Three of a Kind: Three cards of the same value.
Straight: All cards are consecutive values.
Flush: All cards of the same suit.
Full House: Three of a kind and a pair.
Four of a Kind: Four cards of the same value.
Straight Flush: All cards are consecutive values of same suit.
Royal Flush: Ten, Jack, Queen, King, Ace, in same suit.

The cards are valued in the order:
2, 3, 4, 5, 6, 7, 8, 9, 10, Jack, Queen, King, Ace.

If two players have the same ranked hands then the rank made up of the
highest value wins; for example, a pair of eights beats a pair of fives
(see example 1 below). But if two ranks tie, for example, both players
have a pair of queens, then highest cards in each hand are compared
(see example 4 below); if the highest cards tie then the next highest
cards are compared, and so on.

Consider the following five hands dealt to two players:

Hand	 	Player 1	 		Player 2	 			Winner
---------------------------------------------------------------------
1	 		5H 5C 6S 7S KD		2C 3S 8S 8D TD			Player 2
Pair of Fives		Pair of Eights
---------------------------------------------------------------------
2	 		5D 8C 9S JS AC		2C 5C 7D 8S QH			Player 1
Highest card Ace	Highest card Queen
---------------------------------------------------------------------
3			2D 9C AS AH AC		3D 6D 7D TD QD			Player 2
Three Aces			Flush with Diamonds
---------------------------------------------------------------------
4	 		4D 6S 9H QH QC		3D 6D 7H QD QS			Player 1
Pair of Queens		Pair of Queens
Highest card Nine	Highest card Seven
---------------------------------------------------------------------
5	 		2H 2D 4C 4D 4S		3C 3D 3S 9S 9D			Player 1
Full House			Full House
With Three Fours	with Three Threes

The file, poker.txt, contains one-thousand random hands dealt to two players.
Each line of the file contains ten cards (separated by a single space):
the first five are Player 1's cards and the last five are Player 2's cards.
You can assume that all hands are valid (no invalid characters or
repeated cards), each player's hand is in no specific order,
and in each hand there is a clear winner.

How many hands does Player 1 win?

Solution:
*/

#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;

namespace poker {
	struct card {
		char value;
		char suit;

		bool operator< (const card& other) const {
			if (value != other.value)
				return value < other.value;
			return suit < other.suit;
		}
	};

	istream &operator >> (istream &input, card &c) {
		input >> c.value >> c.suit;
		if (c.value == 'T')
			c.value = 10;
		else if (c.value == 'J')
			c.value = 11;
		else if (c.value == 'Q')
			c.value = 12;
		else if (c.value == 'K')
			c.value = 13;
		else if (c.value == 'A')
			c.value = 14;
		else
			c.value -= '0';
		return input;
	}

	struct hand {
		card cards[5];

		auto frequency() const {
			auto freq = vector<pair<char, char>>{ { cards[0].value, 1 } };
			for (size_t i = 1; i < 5; ++i)
				if (cards[i].value == freq.back().first)
					++freq.back().second;
				else
					freq.push_back({ cards[i].value, 1 });

			sort(freq.begin(), freq.end(), [](const auto&p1, const auto&p2) {
				if (p1.second != p2.second)
					return p1.second > p2.second;
				return p1.first > p2.first;
			});

			return freq;
		}

		char consecutive() const {
			if (cards[4].value == 14 && cards[0].value == 2 && cards[1].value == 3
				&& cards[2].value == 4 && cards[3].value == 5)
				return 1;
			if (cards[1].value != cards[0].value + 1)
				return 0;
			if (cards[2].value != cards[0].value + 2)
				return 0;
			if (cards[3].value != cards[0].value + 3)
				return 0;
			if (cards[4].value != cards[0].value + 4)
				return 0;
			return cards[0].value;
		}

		char samesuit() const {
			if (cards[1].suit != cards[0].suit)
				return 0;
			if (cards[2].suit != cards[0].suit)
				return 0;
			if (cards[3].suit != cards[0].suit)
				return 0;
			if (cards[4].suit != cards[0].suit)
				return 0;
			return cards[0].suit;
		}

		int rank() const {
			constexpr int p1 = 20, p2 = p1*p1, p3 = p1*p1*p1, p4 = p2*p2, p5 = p3*p2;
			auto c = consecutive(), s = samesuit();
			// Royal & Straight Flush
			if (c && s)
				return 1 * p5 - c;
			auto f = frequency();
			const auto n = f.size();
			// Four of a Kind
			if (n == 2 && f[0].second == 4)
				return 2 * p5 - p1*f[0].first - f[1].first;
			// Full House
			if (n == 2 && f[0].second == 3 && f[1].second == 2)
				return 3 * p5 - p1*f[0].first - f[1].first;
			// Flush
			if (s)
				return 4 * p5 - p4*cards[4].value - p3*cards[3].value - p2*cards[2].value - p1*cards[1].value - cards[0].value;
			// Straight
			if (c)
				return 5 * p5 - c;
			// Three of a Kind
			if (n == 3 && f[0].second == 3)
				return 6 * p5 - p2*f[0].first - p1*f[1].first - f[2].first;
			// Two Pairs
			if (n == 3)
				return 7 * p5 - p2*f[0].first - p1*f[1].first - f[2].first;
			// One Pair
			if (n == 4)
				return 8 * p5 - p3*f[0].first - p2*f[1].first - p1*f[2].first - f[3].first;
			// High Card
			return 9 * p5 - p4*cards[4].value - p3*cards[3].value - p2*cards[2].value - p1*cards[1].value - cards[0].value;
		}
	};

	istream &operator >> (istream  &input, hand &h) {
		for (auto& card : h.cards)
			input >> card;
		sort(h.cards, h.cards + 5);
		return input;
	}
}

unsigned compute() {
	using namespace poker;
	unsigned count = 0;
	ifstream file(".\\Data\\p054_poker.txt");
	if (file.good()) {
		while (!file.eof()) {
			hand h1, h2;
			file >> h1 >> h2;
			if (!file.eof() && h1.rank() < h2.rank())
				++count;
		}
	}
	else {
		cout << "File not found. \n";
	}
	return count;
}

#ifdef _MSC_VER
	template <class T>
	inline void DoNotOptimize(const T &value) {
		__asm { lea ebx, value }
	}
#else
	template <class T>
	__attribute__((always_inline)) inline void DoNotOptimize(const T &value) {
		asm volatile("" : "+m"(const_cast<T &>(value)));
	}
#endif

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

