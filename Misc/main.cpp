#include <iostream>
#include <string>
#include <limits>
using namespace std;


int main() {
	int q;
	cin >> q;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	for( int i=0; i<q; i++){
		string tmp;
		std::getline(cin, tmp);
	}

	int d;
	cin >> d;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	while (d--){
		string tmp;
		std::getline(cin, tmp);
	}

	int n;
	cin >> n;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	while (n--){
		string tmp;
		std::getline(cin, tmp);
		cout << tmp << " 1" << endl;
	}

	system("pause");
	return 0;
}

/*

3
{"view_count": 773, "question_text": "Which is the most intelligent alien or alien species in fiction?", "context_topic": {"followers": 130960, "name": "Science Fiction (genre)"}, "topics": [{"followers": 48, "name": "Science Fiction Books"}, {"followers": 130960, "name": "Science Fiction (genre)"}, {"followers": 1182, "name": "Extraterrestrial Intelligence"}, {"followers": 50056, "name": "Extraterrestrial Life"}, {"followers": 3883, "name": "Science Fiction Movies"}], "follow_count": 9, "question_key": "AAEAAJ/qtRMKkzXyA0tvjyz5tPRWgYizvOkCr9Z9CdJ4cood", "age": 413}
{"view_count": 3522, "question_text": "What is the best way to keep bookmarks?", "context_topic": {"followers": 513, "name": "Bookmarking"}, "topics": [{"followers": 1136, "name": "Pocket (app)"}, {"followers": 9, "name": "ReadItLater"}, {"followers": 1625, "name": "Pinboard"}, {"followers": 1275, "name": "Social Bookmarking"}, {"followers": 513, "name": "Bookmarking"}, {"followers": 5604, "name": "Delicious (web application)"}, {"followers": 4359, "name": "Instapaper"}, {"followers": 85, "name": "Web Bookmarks"}], "follow_count": 62, "question_key": "AAEAADJKxcVF6l23JZvf1Fz+QrKr35CTlMKayNnZebc8dQAY", "age": 1193}
{"view_count": 390, "question_text": "What is best for online bookmarks?", "context_topic": null, "topics": [{"followers": 1275, "name": "Social Bookmarking"}, {"followers": 285, "name": "Social Bookmarking Websites"}, {"followers": 513, "name": "Bookmarking"}], "follow_count": 4, "question_key": "AAEAAO3FKYrsnYH9uKAOnnXfYrGGTVFA3uzHz+Vltm5Ssii3", "age": 1211}
2
AAEAADJKxcVF6l23JZvf1Fz+QrKr35CTlMKayNnZebc8dQAY AAEAAO3FKYrsnYH9uKAOnnXfYrGGTVFA3uzHz+Vltm5Ssii3 1
AAEAADJKxcVF6l23JZvf1Fz+QrKr35CTlMKayNnZebc8dQAY AAEAAJ/qtRMKkzXyA0tvjyz5tPRWgYizvOkCr9Z9CdJ4cood 0
1
AAEAAJ/qtRMKkzXyA0tvjyz5tPRWgYizvOkCr9Z9CdJ4cood AAEAAO3FKYrsnYH9uKAOnnXfYrGGTVFA3uzHz+Vltm5Ssii3

*/