// floydWarshall & dijkstra

#include <iostream>
#include <climits>
using namespace std;

#define Size 100
#define print(ary,x) for (int i = 0; i < Size; i++)cout << ary[x][i] << " | "; cout << endl

void setShortCut(int x, int y, int adjMat[Size][Size]) {
	for (int j = 1, k = x + 1; j <= 6 && k < Size; j++, k++)
		adjMat[x][k] = INT_MAX;
	adjMat[x][y] = 0;
}

void floydWarshall(int adjMat[Size][Size]) {
	for (int k = 0; k < Size; k++)
		for (int i = 0; i < Size; i++)
			for (int j = 0; j < Size; j++)
				if (adjMat[i][k] != INT_MAX && adjMat[k][j] != INT_MAX && adjMat[i][j] > adjMat[i][k] + adjMat[k][j])
					adjMat[i][j] = adjMat[i][k] + adjMat[k][j];
}

int minDistance(int dist[], bool sptSet[]) {
	int min = INT_MAX, mini = 0;
	for (int v = 0; v < Size; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], mini = v;
	return mini;
}

void dijkstra(int adjMat[Size][Size], int s, int dist[Size]) {
	bool sptSet[Size] = {};
	for (int i = 0; i < Size; i++)
		dist[i] = INT_MAX;
	dist[s] = 0;

	for (int count = 0; count < Size - 1; count++) {
		int u = minDistance(dist, sptSet);
		sptSet[u] = true;

		for (int v = 0; v < Size; v++)
			if (!sptSet[v] && dist[u] != INT_MAX && adjMat[u][v] != INT_MAX && dist[u] + adjMat[u][v] < dist[v])
				dist[v] = dist[u] + adjMat[u][v];
	}
}

int main() {
	std::ios::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;

	while (t--) {
		int adjMat[Size][Size];
		for (int i = 0; i < Size; i++)
			for (int j = 0; j < Size; j++)
				adjMat[i][j] = (i == j) ? 0 : INT_MAX;

		for (int i = 0; i < Size - 1; i++)
			for (int j = i + 1, k = 0; k < 6 && j < Size; j++, k++)
				adjMat[i][j] = 1;

		int n;
		cin >> n;
		for (int i = 0, x, y; i < n; i++) {
			cin >> x >> y;
			setShortCut(--x, --y, adjMat);
		}

		int m;
		cin >> m;
		for (int i = 0, x, y; i < m; i++) {
			cin >> x >> y;
			setShortCut(--x, --y, adjMat);
		}

		int dist[Size];
		dijkstra(adjMat, 0, dist);
		cout << ((dist[99] == INT_MAX) ? -1 : dist[99]) << endl;

		//floydWarshall(adjMat);
		//cout << ((adjMat[0][99] >= INT_MAX) ? -1: adjMat[0][99]) << endl;
	}

#if _MSC_VER >= 1600
	system("pause");
#endif
}