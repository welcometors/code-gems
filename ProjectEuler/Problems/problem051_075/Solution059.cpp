// https://projecteuler.net/problem=59
/*
XOR decryption

Each character on a computer is assigned a unique code and the preferred
standard is ASCII (American Standard Code for Information Interchange).
For example, uppercase A = 65, asterisk (*) = 42, and lowercase k = 107.

A modern encryption method is to take a text file, convert the bytes to ASCII,
then XOR each byte with a given value, taken from a secret key. The advantage
with the XOR function is that using the same encryption key on the cipher text,
restores the plain text; for example, 65 XOR 42 = 107, then 107 XOR 42 = 65.

For unbreakable encryption, the key is the same length as the plain text message,
and the key is made up of random bytes. The user would keep the encrypted message
and the encryption key in different locations, and without both "halves",
it is impossible to decrypt the message.

Unfortunately, this method is impractical for most users, so the modified method
is to use a password as a key. If the password is shorter than the message,
which is likely, the key is repeated cyclically throughout the message.
The balance for this method is using a sufficiently long password key for security,
but short enough to be memorable.

Your task has been made easy, as the encryption key consists of three lower case
characters. Using cipher.txt (right click and 'Save Link/Target As...'), a file
containing the encrypted ASCII codes, and the knowledge that the plain text must
contain common English words, decrypt the message and find the sum of the ASCII
values in the original text.

Solution:

*/

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdint>
#include <chrono>
using namespace std;

auto getText(const string &fileName) {
	std::ifstream file(fileName);
	if (file.good()) {
		char charValue = 0;
		string text, str;
		file >> text;
		text += ',';
		for (char c : text) {
			if (c == ',') {
				str += charValue;
				charValue = 0;
			}
			else
				charValue = charValue * 10 + c - '0';
		}
		return str;
	}
	else {
		cout << "File not found. \n";
	}
	return string();
}

auto decrypt(const string &text) {
	unsigned freq[3][128] = {};
	for (size_t i = 0, n = text.length(); i < n; ++i)
		++freq[i % 3][text[i]];

	string key = "";
	for (size_t i = 0; i < 3; ++i)
		key += (char)((max_element(begin(freq[i]), end(freq[i])) - begin(freq[i])) ^ ' ');
	return key;
}

void showDecoded(const string &text, const string &key) {
	for (size_t i = 0, n = text.length(); i < n; ++i)
		cout << (char)(text[i] ^ key[i % 3]);
	cout << '\n';
}

auto compute() {
	auto text = getText(".\\Data\\p059_cipher.txt");
	auto key = decrypt(text);
	//showDecoded(text, key);
	return key;
}

#ifdef _MSC_VER
	template <class T>
	inline void DoNotOptimize(const T &value) {
		__asm { lea ebx, value }
	}
#else
	template <class T>
	__attribute__((always_inline)) inline void DoNotOptimize(const T &value) {
		asm volatile("" : "+m"(const_cast<T &>(value)));
	}
#endif

int main() {
	using namespace std;
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	DoNotOptimize(result);
	cout << "Done in "
		<< duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6
		<< " miliseconds." << endl;
	cout << result << endl;
}