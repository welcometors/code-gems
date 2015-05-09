// https://www.hackerrank.com/challenges/play-game

#include <iostream>
#include <vector>
#include <memory>
using namespace std;

#define natural unsigned long long
#define MAXTAKE 3

struct gameState{
	natural score;
	int bricks;
};

int main(){
	int t;
	cin >> t;

	while (t--){
		int n;
		cin >> n;

		unique_ptr<natural[]> numbers(new natural[n]);
		
		for (int i = n - 1; i >= 0; i--){
			cin >> numbers[i];
		}

		vector<gameState> score = { gameState{0, 0} };

		for (int i = 0; i < MAXTAKE; i++){
			score.push_back(gameState{ score.back().score + numbers[i], i + 1 });
		}

		for (int i = MAXTAKE; i < n; i++){
			natural take = 0, max = 0;
			int bricks = 0;
			
			for (int j = 0; j < MAXTAKE; j++){
				take += numbers[i - j];
				int opponentBricks = score[i - j].bricks;
				
				if (take + score[i - j - opponentBricks].score > max){
					max = take + score[i - j - opponentBricks].score;
					bricks = j + 1;
				}
			}

			score.push_back(gameState{ max, bricks });
		}

		cout << score.back().score << endl;
	}
    
	return 0;
}

// 1 6 2 5 4 3 2 1
// 2 5 999 1 1 1 0 5 0 1 1 1 999