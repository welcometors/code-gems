#include <set>
#include <string>
#include <vector>
#include <numeric> 
#include <iostream>
#include <algorithm>
using namespace std;

std::string replaceAll(const std::string &str, const std::string& oldString, const std::string& newString) {
	string out;
	size_t startPos = 0, findIdx, templeteLength = oldString.length();
	while ((findIdx = str.find(oldString, startPos)) != std::string::npos) {
		out += str.substr(startPos, findIdx - startPos);
		out += newString;
		startPos = findIdx + templeteLength;
	}
	out += str.substr(startPos);
	return out;
}

struct Email{
	string id;
	string domain;
	Email(const string &emailId){
		auto idx = emailId.find_last_of('@');
		id = "";
		for (auto c : emailId.substr(0, idx)){
			auto lower = tolower(c);
			if (lower == '+')
				break;	// everything after is ignored including plus
			if (lower != '.')
				id += lower;
		}
		domain = emailId.substr(idx + 1);
		transform(domain.begin(), domain.end(), domain.begin(), ::tolower);
	}
};

struct Street{
	string address;
	Street(const string &street){
		address = street;
		transform(address.begin(), address.end(), address.begin(), ::tolower);
		address = replaceAll(address, "street", "st.");
		address = replaceAll(address, "road", "rd.");
	}
};

struct State{
	string name;
	State(const string &state){
		name = state;
		transform(name.begin(), name.end(), name.begin(), ::tolower);
		name = replaceAll(name, "illinois", "il");
		name = replaceAll(name, "california", "ca");
		name = replaceAll(name, "new york", "ny");
	}
};

struct ZipCode{
	string code;
	ZipCode(const string &zipcode){
		code = "";
		for (auto c : zipcode)
			if ('0' <= c && c <= '9')
				code += c;
	}
};

class Order{
public:
	int orderId;
	int dealId;
	Email emailId;
	Street street;
	string city;
	State state;
	ZipCode zipcode;
	string creditCard;
	string completeEmailId;
	string address;

	Order(const vector<string> &row)
		: orderId(stoi(row[0])), dealId(stoi(row[1])), emailId(row[2])
		, street(row[3]), city(row[4]), state(row[5]), zipcode(row[6]), creditCard(row[7]){
		transform(city.begin(), city.end(), city.begin(), ::tolower);
		completeEmailId = emailId.id + '@' + emailId.domain;
		address = street.address + ' ' + city + ' ' + state.name + ' ' + zipcode.code;
	}

	friend ostream &operator << (ostream &output, const Order &o){
		output << "{" << o.orderId << ", " << o.dealId << ", '" << o.completeEmailId << "', '"
			<< o.address << "', " << o.zipcode.code << ", " << o.creditCard << "}";
		return output;
	}

	struct FraudulentOrderType1Comparator : public binary_function<Order, Order, bool>
	{
		bool operator()(const Order& o1, const Order& o2) const {
			if (o1.dealId != o2.dealId)
				return o1.dealId < o2.dealId;
			return o1.completeEmailId < o2.completeEmailId;
		}
	};

	struct FraudulentOrderType2Comparator : public binary_function<Order, Order, bool>
	{
		bool operator()(const Order& o1, const Order& o2) const {
			if (o1.dealId != o2.dealId)
				return o1.dealId < o2.dealId;
			return o1.address < o2.address;
		}
	};
};

set<int> getFraudulents(vector<Order> &orders){
	/* Types of fraudulent order
	1) Two orders have the same e-mail address and deal id
	2) Two orders have the same Address/City/State/Zip and deal id
	*/
	set<int> fraudulentIds;

	// sort to detect Type-1 fraud
	sort(orders.begin(), orders.end(), Order::FraudulentOrderType1Comparator());

	for (int i = 1, n = orders.size(); i < n; i++){
		if (orders[i].dealId == orders[i - 1].dealId && orders[i].completeEmailId == orders[i - 1].completeEmailId){
			fraudulentIds.insert(orders[i - 1].orderId);
			fraudulentIds.insert(orders[i].orderId);
		}
	}

	// sort to detect Type-2 fraud
	sort(orders.begin(), orders.end(), Order::FraudulentOrderType2Comparator());

	for (int i = 1, n = orders.size(); i < n; i++){
		if (orders[i].dealId == orders[i - 1].dealId && orders[i].address == orders[i - 1].address){
			fraudulentIds.insert(orders[i - 1].orderId);
			fraudulentIds.insert(orders[i].orderId);
		}
	}

	return fraudulentIds;
}

vector<string> split(const string &s, const char delimiter){
	vector<string> tokens;
	string token = "";

	for (auto c : s){
		if (c == delimiter && token.length()){
			tokens.push_back(token);
			token.clear();
		}
		else
			token += c;
	}

	if (token.length())
		tokens.push_back(token);

	return tokens;
}

int main() {
	int n;
	cin >> n;
	cin.ignore();

	vector<Order> orders;
	while (n--){
		string orderRow;
		getline(cin, orderRow);
		orders.emplace_back(split(orderRow, ','));
	}

	auto fraudulents = getFraudulents(orders);
	cout <<
		accumulate(fraudulents.begin(), fraudulents.end(), std::string(),
		[](const string& a, const int& b) -> std::string {
			return a + (a.length() > 0 ? "," : "") + to_string(b);
	}) << endl;

	system("pause");
	return 0;
}

/*
6
1,58765,emardl@aol.com,9620 Emmerich Manors,West Roberts,NY,88152-2622,2524520704
2,31376,eleanore_kirlin@yahoo.com,673 Collier Road,Arveltown,CA,56605-6003,8211047638
3,58765,emardl@aol.com,9621 Emmerich Manors,West Roberts,NY,88152-2622,2524520704
4,58765,emardl@aol.com,9622 Emmerich Manors,West Roberts,NY,88152-2622,3624620704
5,58765,emardl@aol.com,9623 Emmerich Manors,West Roberts,NY,88152-2622,3624620704
6,58765,emardl@aol.com,9624 Emmerich Manors,West Roberts,NY,88152-2622,7524720704
*/