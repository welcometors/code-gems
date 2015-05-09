#include <cstdio>

#define INPUTBUFFERSIZE 65535
class FastIO{
	static char inputBuffer[INPUTBUFFERSIZE];
	static bool isWhiteSpace[33];
	static int inputBufferIndex;
	static int inputBufferLength;
	static bool endOfInput;

	static bool readInputBuffer(){
		if (endOfInput)
			return endOfInput;

		inputBufferLength = fread(inputBuffer, sizeof(char), INPUTBUFFERSIZE, stdin);
		if (inputBufferLength > 0)
			inputBufferIndex = 0;
		else{
			inputBufferLength = 0;
			return endOfInput = true;
		}

		return false;
	}

public:

	inline static bool eof(){
		return endOfInput;
	}

	static bool moveToNextNonWhiteSpace(){
		do{
			while (inputBufferIndex < inputBufferLength && inputBuffer[inputBufferIndex] <= 32 && isWhiteSpace[inputBuffer[inputBufferIndex]])
				inputBufferIndex++;

			if (inputBufferIndex >= inputBufferLength){
				if (readInputBuffer())
					return false;
			}
			else
				return true;
		} while (true);
		return false;
	}

	static char readChar(){
		if (inputBufferIndex >= inputBufferLength){
			if (readInputBuffer())
				return '\n';
		}
		return inputBuffer[inputBufferIndex++];
	}

	static bool readInt(int *i){
		if (moveToNextNonWhiteSpace()){
			char c = readChar();
			bool s = false;
			int b = 10;
			*i = 0;

			if (c == '-'){
				s = true;
				c = readChar();
			}
			if (c == '0'){
				b = 8;
				c = readChar();
			}
			if (c == 'x'){
				b = 16;
				c = readChar();
			}
			switch (b){
				case 8:
					while ('0' <= c && c <= '7'){
						*i = *i * b + c - '0';
						c = readChar();
					}
					break;
				case 16:
					while (true){
						if ('0' <= c && c <= '9')
							*i = *i * b + c - '0';
						else if ('a' <= c && c <= 'f')
							*i = *i * b + c - 'a' + 10;
						else if ('A' <= c && c <= 'F')
							*i = *i * b + c - 'A' + 10;
						else
							break;
						c = readChar();
					}
					break;
				default:
					while ('0' <= c && c <= '9'){
						*i = *i * b + c - '0';
						c = readChar();
					}
					break;
			}
			if (s)
				*i = -*i;
			return true;
		}
		return false;
	}
};

char FastIO::inputBuffer[INPUTBUFFERSIZE];
bool FastIO::isWhiteSpace[33] = {	   false, false, false, false, false,
	false, false, false, false,  true,  true,  true,  true,  true, false, 
	false, false, false, false, false, false, false, false, false, false, 
	false, false, false, false, false, false, false,  true};
int FastIO::inputBufferIndex = 0;
int FastIO::inputBufferLength = 0;
bool FastIO::endOfInput = false;

int main(){
	int n, k, c = 0, t;
	FastIO::readInt(&n);
	FastIO::readInt(&k);

	for (; n--;){
		FastIO::readInt(&t);
		if (t % k == 0)
			c++;
	}

	printf("%d\n", c);
	/*int n;
	while (FastIO::readInt(&n)){
		if (n == 42)
			break;
		printf("%d\n", n);
	}*/
}

// 7 3 1 51 966369 7 9 999996 11 