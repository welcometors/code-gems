// https://www.hackerrank.com/contests/quora-haqathon/challenges/upvotes

#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <cstdint>
#include <algorithm>
using namespace std;

#define localdebug

struct quoraDay{
	uint32_t upvotes;
	int32_t nonDecChainLen;
	int32_t nonDecChainEnd;
	int32_t nonIncChainLen;
	int32_t nonIncChainEnd;
};

#ifdef localdebug

#define printField( ary, n, field)		\
	for (int i = 0; i<n; i++)			\
		cout<< ary[i].field << "\t";	\
	cout << endl

void getUpvoteWindowsBF(stringstream &sout, unique_ptr<quoraDay[]> &days, int n, int k){
	for (int i = 0; i < n - k + 1; i++){
		int64_t nonDec = 0, nonInc = 0;
		
		for (int l = 2; l <= k; l++){
			for (int j = 0; j < k - l + 1; j++){
				bool isNonDesc = true, isNonInc = true;
				
				for (int x = 1; (isNonDesc || isNonInc) && x < l; x++){
					if (days[i + j + x - 1].upvotes > days[i + j + x].upvotes)
						isNonDesc = false;
					else if (days[i + j + x - 1].upvotes < days[i + j + x].upvotes)
						isNonInc = false;
				}
				
				if (isNonDesc)
					nonDec++;
				if (isNonInc)
					nonInc++;
			}
		}
		
		sout << (int64_t)(nonDec - nonInc) << endl;
	}
}

#endif

void getUpvoteWindows(stringstream &sout, unique_ptr<quoraDay[]> &days, int n, int k, bool debug = false){
	if (k == 1){
		for (int i = 0; i < n; i++){
			sout << "0" << endl;
		}
		return;
	}

	days[0].nonDecChainLen = 0;
	days[n - 1].nonIncChainLen = 0;

	for (int i = 1, j = n - 2; i < n; i++, j--){
		if (days[i - 1].upvotes <= days[i].upvotes){
			days[i].nonDecChainLen = days[i - 1].nonDecChainLen + 1;
		}
		else{
			days[i].nonDecChainLen = 0;
		}

		if (days[j + 1].upvotes <= days[j].upvotes){
			days[j].nonIncChainLen = days[j + 1].nonIncChainLen + 1;
		}
		else{
			days[j].nonIncChainLen = 0;
		}
	}

	days[n - 1].nonDecChainEnd = n - 1;
	days[0].nonIncChainEnd = 0;

	for (int i = n - 2, j = 1; i >= 0; i--, j++){
		if (days[i].upvotes <= days[i + 1].upvotes){
			days[i].nonDecChainEnd = days[i + 1].nonDecChainEnd;
		}
		else{
			days[i].nonDecChainEnd = i;
		}

		if (days[j].upvotes <= days[j - 1].upvotes){
			days[j].nonIncChainEnd = days[j - 1].nonIncChainEnd;
		}
		else{
			days[j].nonIncChainEnd = j;
		}
	}

#ifdef localdebug
	if (debug){
		printField(days, n, upvotes);
		printField(days, n, nonDecChainLen);
		printField(days, n, nonIncChainLen);
		printField(days, n, nonDecChainEnd);
		printField(days, n, nonIncChainEnd);
	}
#endif

	unique_ptr<int64_t[]> nonDecSubRanges(new int64_t[n - k + 1]);
	unique_ptr<int64_t[]> nonIncSubRanges(new int64_t[n - k + 1]);
	int64_t decSubRange = 0, incSubRange = 0;

	for (int i = 0, j = n - 1; i < k; i++, j--){
		decSubRange += days[i].nonDecChainLen;
		incSubRange += days[j].nonIncChainLen;
	}

	nonDecSubRanges[0] = decSubRange;
	nonIncSubRanges[n - k] = incSubRange;

	for (int i = k, j = n - 1 - k; i < n; i++, j--){
		int32_t decChainEnd = (days[i - k].nonDecChainEnd > i) ? i : days[i - k].nonDecChainEnd;
		if (decChainEnd != i){
			decSubRange += days[i].nonDecChainLen;
			decSubRange -= days[decChainEnd].nonDecChainLen - days[i - k].nonDecChainLen;
		}
		nonDecSubRanges[i - k + 1] = decSubRange;

		int32_t incChainEnd = (days[j + k].nonIncChainEnd < j) ? j : days[j + k].nonIncChainEnd;
		if (incChainEnd != j){
			incSubRange += days[j].nonIncChainLen;
			incSubRange -= days[incChainEnd].nonIncChainLen - days[j + k].nonIncChainLen;
		}
		nonIncSubRanges[j] = incSubRange;
	}

	for (int i = 0, j = n - k; j >= 0; i++, j--){
		sout << (int64_t)(nonDecSubRanges[i] - nonIncSubRanges[i]) << endl;

#ifdef localdebug
		if (debug){
			std::cout << nonDecSubRanges[i] << " - " << nonIncSubRanges[i] << " = " 
				<< (int64_t)(nonDecSubRanges[i] - nonIncSubRanges[i]) << endl;
		}
#endif

	}
}

#ifdef localdebug

void testA(int t, int maxSize, int maxUpvotes){
	int failed = 0;

	for (int i = 0; i < t; i++){
		int n = 1 + (rand() % maxSize);
		int k = 1 + (rand() % n);

		unique_ptr<quoraDay[]> days(new quoraDay[n]);

		for (int j = 0; j < n; j++){
			days[j].upvotes = 1 + (rand()%maxUpvotes);
		}

		stringstream a;
		stringstream b;
		
		getUpvoteWindowsBF(b, days, n, k);
		getUpvoteWindows(a, days, n, k);
		
		string bStr = b.str();
		string aStr = a.str();

		a.clear(); 
		b.clear();
		
		if (aStr != bStr){
			if (!failed){
				cout << "Failed Input: " << endl;
				
				for (int j = 0; j < n; j++)
					cout << days[j].upvotes << ", ";
				
				cout << endl << " (" << k << ") " << endl;
				
				std::replace(aStr.begin(), aStr.end(), '\n', ' ');
				std::replace(bStr.begin(), bStr.end(), '\n', ' ');
				cout << "BruteForce: " << bStr << endl;
				cout << "Algorithm: " << aStr << endl;
				
				a.clear();
				cout << "debug" << endl;
				getUpvoteWindows(a, days, n, k, true);
			}

			failed++;
		}
	}

	cout << failed << " of " << t << " failed. " << 100.0 * (float)(t - failed) / (t) << "% passed." << endl;
}

#endif

int main(){
#ifdef localdebug
	testA(1000, 8, 10);
#endif
	int n, k;
	cin >> n >> k;

	unique_ptr<quoraDay[]> days(new quoraDay[n]);
	stringstream sin;
	sin << cin.rdbuf();

	for (int i = 0; i < n; i++){
		sin >> days[i].upvotes;
	}

	stringstream sout;
	getUpvoteWindows(sout, days, n, k);
	std::cout << sout.rdbuf();

#ifdef localdebug
	system("pause");
#endif
	return 0;
}

// 5 5 1 2 3 1 1
// 7 3 1 2 3 3 2 1 3
