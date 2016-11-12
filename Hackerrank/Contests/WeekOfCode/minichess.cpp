#include <iostream>
#include <vector>
#include <algorithm>

namespace minichess {
	constexpr int BoardSize = 4;
	constexpr int Queen = 4;
	constexpr int Rook = 3;
	constexpr int Bishop = 2;
	constexpr int Knight = 1;
	constexpr int ValueBits = 3;
	constexpr int White = 1 << ValueBits;

	const int infinity = 999999;
	const int weightTable[] = { 0,6,7,12,1000 };

	struct position {
		int x, y;
	};
	using move = std::pair<position, position>;
	const std::vector<position> straight = { {1,0}, {0,1}, {-1,0},{0,-1} };
	const std::vector<position> cross = { { 1,1 },{ -1,1 },{ -1,-1 },{ 1,-1 } };
	const std::vector<position> horse = { { 2,1 },{ 2,-1 },{ 1,2 },{ 1,-2 },{ -1,2 },{ -1,-2 },{ -2,1 },{ -2,-1 } };

	inline int getPiece(const int value) {
		return value & (White - 1);
	}

	inline bool getColor(const int value) {
		return value >> ValueBits;
	}

	struct Board {
		int values[BoardSize][BoardSize] = {};

		void put(char piece, int x, int y, bool isWhite) {
			if (piece == 'Q')
				values[y][x] = Queen;
			else if (piece == 'R')
				values[y][x] = Rook;
			else if (piece == 'N')
				values[y][x] = Knight;
			else if (piece == 'B')
				values[y][x] = Bishop;
			if (isWhite)
				values[y][x] |= White;
		}

		bool pushMoves(const position& start, std::vector<move>& allMoves, const std::vector<position>& pattern, bool repeat = true) {
			const auto color = getColor(values[start.y][start.x]);
			for (const auto& step : pattern) {
				auto current = start;
				do {
					current.x += step.x;
					current.y += step.y;
					if (current.x < 0 || current.x >= BoardSize || current.y < 0 || current.y >= BoardSize)
						break;
					const int value = values[current.y][current.x];
					if (value && getColor(value) == color)
						break;
					if (getPiece(value) == Queen)
						return true;
					allMoves.push_back({ start,current });
					if (value)
						break;
				} while (repeat);
			}
			return false;
		}

		int abminmax(int depth, int alpha, int beta, bool maximizingPlayer) {
			position queen{ -1,-1 };
			std::vector<position> rooks;
			std::vector<position> bishops;
			std::vector<position> knights;

			int score = 0;
			for (int y = 0; y < BoardSize; ++y) {
				for (int x = 0; x < BoardSize; ++x) {
					const auto color = getColor(values[y][x]);
					const auto piece = getPiece(values[y][x]);
					score += color ? weightTable[piece] : -weightTable[piece];
					if (maximizingPlayer == color) {
						if (piece == Queen)
							queen = { x,y };
						else if (piece == Rook)
							rooks.push_back({ x,y });
						else if (piece == Knight)
							knights.push_back({ x,y });
						else if (piece == Bishop)
							bishops.push_back({ x,y });
					}
				}
			}

			if (!depth)
				return score;

			std::vector<move> moves;
			if (pushMoves(queen, moves, straight) || pushMoves(queen, moves, cross))
				return maximizingPlayer ? +infinity : -infinity;
			for (const auto& rook : rooks)
				if (pushMoves(rook, moves, straight))
					return maximizingPlayer ? +infinity : -infinity;
			for (const auto& knight : knights)
				if (pushMoves(knight, moves, horse, false))
					return maximizingPlayer ? +infinity : -infinity;
			for (const auto& bishop : bishops)
				if (pushMoves(bishop, moves, cross))
					return maximizingPlayer ? +infinity : -infinity;

			std::sort(moves.begin(), moves.end(), [&](const auto& m1, const auto& m2) {
				return getPiece(values[m1.second.y][m1.second.x]) > getPiece(values[m2.second.y][m2.second.x]);
			});

			score = maximizingPlayer ? -infinity : +infinity;
			for (const auto& m : moves) {
				int save = values[m.second.y][m.second.x];
				values[m.second.y][m.second.x] = values[m.first.y][m.first.x];
				values[m.first.y][m.first.x] = 0;
				auto result = abminmax(depth - 1, alpha, beta, !maximizingPlayer);
				if ((maximizingPlayer && result > score) || (!maximizingPlayer && result < score))
					score = result;
				if (maximizingPlayer && score > alpha)
					alpha = score;
				if (!maximizingPlayer && score < beta)
					beta = score;
				values[m.first.y][m.first.x] = values[m.second.y][m.second.x];
				values[m.second.y][m.second.x] = save;
				if (beta <= alpha)
					break;	//prune
			}
			return score;
		}

		bool solve(int depth) {
			return abminmax(depth, -infinity, +infinity, true) == infinity;
		}
	};
}

int main() {
	using namespace std;
	using namespace minichess;
	ios::sync_with_stdio(false);
	cin.tie(0);

	int g;
	cin >> g;
	while (g--) {
		Board board;
		int w, b, m;
		cin >> w >> b >> m;
		for (int i = w + b; i; i--) {
			char v, c, r;
			cin >> v >> c >> r;
			board.put(v, c - 'A', r - '1', i > b);
		}
		cout << (board.solve(m) ? "YES" : "NO") << '\n';
	}
}

/*
1
2 1 1
N B 2
Q B 1
Q A 4

1
1 1 1
Q A 1
Q C 4

1
2 1 5
N B 2
Q B 1
Q D 4

1
2 1 3
R B 2
Q C 1
Q D 4
*/