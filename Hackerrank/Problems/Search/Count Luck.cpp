// https://www.hackerrank.com/challenges/count-luck

#include <iostream>
#include <string>
#include <memory>
using namespace std;

#define UNPASSABLE 'X'
#define   PASSABLE '.'
#define STARTSTATE 'M'
#define   ENDSTATE '*'
#define    DEADEND '#'

#define printJungle(jungle, R, C)						\
	for (int i = 0, k = 0; i < R; i++, cout << endl){	\
		for (int j = 0; j < C; j++, k++){				\
			cout << jungle[k];	}}

#define move( jungle, block, C)							\
	if (jungle[block - 1] == PASSABLE)					\
		block--;										\
	else if (jungle[block + 1] == PASSABLE)				\
		block++;										\
	else if (jungle[block - (C)] == PASSABLE)			\
		block -= (C);									\
	else if (jungle[block + (C)] == PASSABLE)			\
		block += (C);									\
	else												\
		block = 0

#define neighbours( jungle, block, C, type)				\
	(((jungle[block - 1] == type) ? 1 : 0) +			\
	((jungle[block + 1] == type) ? 1 : 0) +				\
	((jungle[block - (C)] == type) ? 1 : 0) +			\
	((jungle[block + (C)] == type) ? 1 : 0))


int main(){
	int t;
	cin >> t;

	while (t--){
		int r, c;
		cin >> r >> c;

		unique_ptr<char[]> jungle(new char[(r + 2) * (c + 2)]);
		
		// creating an unpassable boundry around jungle
		for (int i = 0; i < c + 2; i++){
			jungle[i] = jungle[(r + 1)*(c + 2) + i] = UNPASSABLE;
		}
		
		for (int i = 0; i < r + 2; i++){
			jungle[i*(c + 2)] = jungle[i*(c + 2) + c + 1] = UNPASSABLE;
		}

		// input forest
		for (int i = 0, j = c + 3; i < r; i++, j += 2){
			string s;
			cin >> s;
			
			for ( auto c: s)
				jungle[j++] = c;
		}

		int start = 0;

		// fill dead ends so that there will be only one path
		for (int i = 1, k = c + 3; i < r + 1; i++, k += 2){
			for (int j = 1; j < c + 1; j++, k++){
				if (jungle[k] == STARTSTATE)
					start = k;

				if (jungle[k] == PASSABLE){
					int block = k;

					do{
						int ends = neighbours(jungle, block, c + 2, UNPASSABLE) + neighbours(jungle, block, c + 2, DEADEND);

						if (ends == 3 && jungle[block] == PASSABLE){
							jungle[block] = DEADEND;
							move(jungle, block, c + 2);
						}
						else
							break;

					}while (block);
				}
			}
		}

		// printJungle(jungle, r + 2, c + 2);
		int magicCount = 0;
		
		while (start && jungle[start] != ENDSTATE){
			if (neighbours(jungle, start, c + 2, DEADEND) > 0)
				magicCount++;
			
			jungle[start] = UNPASSABLE;
			move(jungle, start, c + 2);
		}

		int k;
		cin >> k;
		cout << ((k == magicCount) ? "Impressed" : "Oops!") << endl;
	}

	system("pause");
	return 0;
}

// 3 2 3 *.M .X. 1 4 11 .X.X......X .X*.X.XXX.X .XX.X.XM... ......XXXX. 3 4 11 .X.X......X .X*.X.XXX.X .XX.X.XM... ......XXXX. 4
// 1 4 11 .X.X......X .X*.X.XXX.X .XX.X.XM... ......XXXX. 4