// https://www.hackerrank.com/contests/quora-haqathon/challenges/schedule

#include <limits>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define real double
#define natural int
#define localTest

struct test{
	natural time;	// running time
	real prob;		// probability of passing
	real expt;		// estimate
};

#ifdef localTest

vector<natural> factorials = { 1 };
natural factorial(int n){
	while (n >= factorials.size())
		factorials.push_back(factorials.back() * factorials.size());

	return factorials[n];
}

template<typename T>
vector<T> getKthPermutation(const vector<T> &input, size_t k){
	if (input.size() < 2){
		return input;
	}

	auto output = input;

	for (size_t i = 0; k && i < input.size() - 1; i++){
		size_t f = factorial(input.size() - 1 - i);
		size_t j = k / f;

		if (j){
			std::iter_swap(output.begin() + i, output.begin() + i + j);
		}

		k = k % f;
	}

	return output;
}

real minimizeExpectanceBF(const vector<test> &tests){
	real optimal = numeric_limits<real>::max();
	int permutations = factorial(tests.size());

	for (int i = 0; i < permutations; i++){
		auto permut = getKthPermutation(tests, i);

		real expt = 0;
		for (auto t : permut)
			expt = t.time + t.prob * expt;

		if (expt < optimal)
			optimal = expt;
	}

	return optimal;
}

#endif

real minimizeExpectance(vector<test> &tests, bool debug = false){
	for (int i = 1; i < tests.size(); i++){
		test current = tests[i];
		int j;

		for (j = 0; j < i; j++){
			real exptIfInserted = current.time + current.prob * tests[j].expt;
			real exptIfForwarded = current.time + current.prob * ((tests[j].expt - tests[j].time) / ((tests[j].prob == 0.0) ? 1.0 : tests[j].prob));
			
			if (exptIfInserted < tests[j].time + tests[j].prob*exptIfForwarded)
				break;

			tests[j].expt = tests[j].time + tests[j].prob*exptIfForwarded;
		}

		for (int k = i; k > j; k--)
			tests[k] = tests[k - 1];

		if (j != i){
			tests[j] = current;
			tests[j].expt = tests[j].time + tests[j].prob*tests[j + 1].expt;
		}

		for (int k = j - 1; k >= 0; k--){
			tests[k].expt = tests[k].time + tests[k].prob*tests[k + 1].expt;
		}
#ifdef localTest
		if (debug){
			for (int k = 0; k <= i; k++){
				cout << "( " << tests[k].time << ", " << tests[k].expt << " ) ";
			}
			cout << endl;
		}
#endif
	}

	return tests[0].expt;
}

#ifdef localTest

void testA(int t, int maxSize, int maxTime){
	int failed = 0;

	for (int i = 0; i < t; i++){
		vector<test> sample;
		
		for (int j = 0; j < maxSize; j++){
			int time = 1 + (rand() % maxTime);
			real prob = (rand() % 101) / 100.0;
			sample.push_back(test{ time, prob, time });
		}

		real b = minimizeExpectanceBF(sample);
		auto sampleCopy = sample;
		real a = minimizeExpectance(sampleCopy);

		if (a != b){
			if (!failed){
				cout << "Failed Input: " << endl;
				cout << sample.size() << " ";
				for (auto s : sample)
					cout << s.time << " " << s.prob << " ";
				cout << endl << "BruteForce: " << b << ", Algorithm: " << a << endl;
				cout << "debug" << endl;
				minimizeExpectance(sample, true);
			}
			failed++;
		}
	}

	cout << failed << " of " << t << " failed. " << 100.0 * (float)(t - failed) / (t) << "% passed." << endl;
}

#endif

int main(){

#ifdef localTest
	testA(1000, 4, 10);
#endif

	int n;
	cin >> n;

	vector<test> tests;

	for (int i = 0; i < n; i++){
		test x;
		cin >> x.time >> x.prob;
		x.expt = x.time;
		tests.push_back(x);
	}

	cout << setprecision(15) << minimizeExpectance(tests) << endl;

#ifdef localTest
	system("pause");
#endif
	return 0;
}

// 3 3 0.1 7 0.5 9 0.2
// 3 91 0.805414173 79 0.739218120 61 0.940068379
