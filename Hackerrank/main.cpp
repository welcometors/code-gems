#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
using namespace std;

typedef struct {
	string id;
	int weight;
	int value;
} parcel;

// sorted by weight
vector<parcel> parcels = {
	{ "p0", 3, 3 } ,
	{ "p1", 4, 22 },
	{ "p2", 5, 5 } ,
	{ "p3", 6, 6 } ,
	{ "p4", 7, 8 } ,
	{ "p5", 8, 9 } ,
	{ "p6", 9, 8 } ,
	{ "p7", 10, 9 }
};

int main(){
	int loadLimit = 19;
	int totalParcels = parcels.size();
	int *loadTable = new int[(loadLimit + 1) * (totalParcels + 1)];
	int *dirTable = new int[(loadLimit + 1) * (totalParcels + 1)];

	for (int j = 0; j <= loadLimit; j++){
		loadTable[j] = dirTable[j] = 0;
	}

	for (int i = 1; i <= totalParcels; i++){
		for (int j = 0; j <= loadLimit; j++){
			int a = loadTable[(i - 1)*(loadLimit + 1) + j];
			if (parcels[i - 1].weight <= j){
				int b = loadTable[(i - 1)*(loadLimit + 1) + j - parcels[i - 1].weight] + parcels[i - 1].value;
				if (a < b){
					loadTable[i*(loadLimit + 1) + j] = b;
					dirTable[i*(loadLimit + 1) + j] = 1;
				}
				else{
					loadTable[i*(loadLimit + 1) + j] = a;
					dirTable[i*(loadLimit + 1) + j] = 0;
				}
			}
			else{
				loadTable[i*(loadLimit + 1) + j] = a;
				dirTable[i*(loadLimit + 1) + j] = 0;
			}
		}
	}

	/*for (int i = 1; i <= totalParcels; i++){
		for (int j = 1; j <= loadLimit; j++){
			cout << dirTable[i*(loadLimit + 1) + j] << " ";
		}
		cout << endl;
	}*/

	int x = loadLimit, y = totalParcels;
	while (x > 0 && y > 0){
		if (dirTable[y*(loadLimit + 1) + x]){
			cout << parcels[y - 1].id << "\t" << parcels[y - 1].weight << "\t" << parcels[y - 1].value << endl;
			x -= parcels[y - 1].weight;
		}
		y--;
	}

	cout << "total value = " << loadTable[totalParcels*(loadLimit + 1) + loadLimit] << endl;

	delete[] loadTable;
	delete[] dirTable;

#ifdef _MSC_VER
	std::system("pause");
#endif

	return 0;
}
