// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/maximum-size-sub-matrix-with-all-1s-in-a-binary-matrix/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using matrix = vector<vector<int>>;

ostream& operator << (ostream& stream, const matrix& mat){
	for (auto row : mat) {
		for (auto elem : row)
			stream << elem << ' ';
		stream << '\n';
	}
	return stream;
}

int maximumSubSquareMatrixWithAll1s(const matrix& mat) {
	size_t rows = mat.size();
	size_t columns = mat.front().size();
	matrix out(2, mat[0]);
	int max = *max_element(mat[0].begin(), mat[0].end());

	for (int r = 1; r < rows; r++) {
		out[r & 1][0] = mat[r][0];
		for (int c = 1; c < columns; c++)
			out[r & 1][c] = mat[r][c] ?
				min({ out[r & 1][c - 1], out[(r + 1) & 1][c - 1], out[(r + 1) & 1][c] }) + 1
				: 0;
		int localMax = *max_element(out[r & 1].begin(), out[r & 1].end());
		if (localMax > max)
			max = localMax;
	}

	return max;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	matrix mat = {
		{0,1,1,0,1},
		{1,1,0,1,0},
		{0,1,1,1,0},
		{1,1,1,1,0},
		{1,1,1,1,1},
		{0,0,0,0,0}};

	cout << mat << '\n';
	cout << maximumSubSquareMatrixWithAll1s(mat) << endl;
}