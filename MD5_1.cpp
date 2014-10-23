// MD5_1.cpp: определяет точку входа для консольного приложения.
//


#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

bool getFileContent(char* fileName, vector<unsigned char> &fileContent);
void alignment(vector<unsigned char> &fileContent);
void Rounds(unsigned long *buf, unsigned long *segment_message);
const char* getHash(vector<unsigned char> inputContent);

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((~z) & (y)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((~z) | (x)))

#define displacement(x, s) (((x) << (s)) | ((x) >> (32-(s))))// смещение влево на s ,бит

#define calculation(Fun, a, b, c, d, x, s, ac) {\
  (a) += Fun((b), (c), (d)) + (x) + (unsigned int)(ac);\
  (a) = displacement((a), (s));\
  (a) += (b);\
 }
int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	vector<unsigned char> inputContent;
	if (!getFileContent(argv[1], inputContent))
	{
		cerr << "Can't open input file: " << endl;
		return -1;
	}

	cout << getHash(inputContent);

	return 0;
}

bool getFileContent(char* fileName, vector<unsigned char> &fileContent)
{
	ifstream inputStream(fileName, std::ios::binary);
	if (inputStream.fail())
		return false;

	inputStream.seekg(0, std::ios::end);
	int inputSize = inputStream.tellg();
	inputStream.seekg(0, std::ios::beg);

	if (!inputSize)
	{
		fileContent.clear();
		return true;
	}

	fileContent.resize(inputSize);
	inputStream.read((char*)&fileContent.front(), inputSize);
	inputStream.close();

	return true;
}

void alignment(vector<unsigned char> &fileContent)
{
	size_t size = fileContent.size() * 8;
	fileContent.push_back(0x80);

	int nZeroes = 56 - fileContent.size() % 64;
	if (nZeroes < 0)
		nZeroes += 64;
	fileContent.insert(fileContent.end(), nZeroes, 0);

	for (int i = 0; i < 4; i++)
		fileContent.push_back(size >> i * 8);
	fileContent.insert(fileContent.end(), 4, 0);
}

void Rounds(unsigned long *buf, unsigned long *segment_message)
{
	unsigned long a = buf[0];
	unsigned long b = buf[1];
	unsigned long c = buf[2];
	unsigned long d = buf[3];

	#define S11 7
	#define S12 12
	#define S13 17
	#define S14 22
	calculation (F, a, b, c, d, segment_message[ 0], S11, 0xD76AA478L);
	calculation (F, d, a, b, c, segment_message[ 1], S12, 0xE8C7B756L);
	calculation (F, c, d, a, b, segment_message[ 2], S13, 0x242070DBL);
	calculation (F, b, c, d, a, segment_message[ 3], S14, 0xC1BDCEEEL);
	calculation (F, a, b, c, d, segment_message[ 4], S11, 0xF57C0FAFL);
	calculation (F, d, a, b, c, segment_message[ 5], S12, 0x4787C62AL);
	calculation (F, c, d, a, b, segment_message[ 6], S13, 0xA8304613L);
	calculation (F, b, c, d, a, segment_message[ 7], S14, 0xFD469501L);
	calculation (F, a, b, c, d, segment_message[ 8], S11, 0x698098D8L);
	calculation (F, d, a, b, c, segment_message[ 9], S12, 0x8B44F7AFL);
	calculation (F, c, d, a, b, segment_message[10], S13, 0xFFFF5BB1L);
	calculation (F, b, c, d, a, segment_message[11], S14, 0x895CD7BEL);
	calculation (F, a, b, c, d, segment_message[12], S11, 0x6B901122L);
	calculation (F, d, a, b, c, segment_message[13], S12, 0xFD987193L);
	calculation (F, c, d, a, b, segment_message[14], S13, 0xA679438EL);
	calculation (F, b, c, d, a, segment_message[15], S14, 0x49B40821L);

	#define S21 5
	#define S22 9
	#define S23 14
	#define S24 20
	calculation (G, a, b, c, d, segment_message[ 1], S21, 0xF61E2562L);
	calculation (G, d, a, b, c, segment_message[ 6], S22, 0xC040B340L);
	calculation (G, c, d, a, b, segment_message[11], S23, 0x265E5A51L);
	calculation (G, b, c, d, a, segment_message[ 0], S24, 0xE9B6C7AAL);
	calculation (G, a, b, c, d, segment_message[ 5], S21, 0xD62F105DL);
	calculation (G, d, a, b, c, segment_message[10], S22, 0x02441453L);
	calculation (G, c, d, a, b, segment_message[15], S23, 0xD8A1E681L);
	calculation (G, b, c, d, a, segment_message[ 4], S24, 0xE7D3FBC8L);
	calculation (G, a, b, c, d, segment_message[ 9], S21, 0x21E1CDE6L);
	calculation (G, d, a, b, c, segment_message[14], S22, 0xC33707D6L);
	calculation (G, c, d, a, b, segment_message[ 3], S23, 0xF4D50D87L);
	calculation (G, b, c, d, a, segment_message[ 8], S24, 0x455A14EDL);
	calculation (G, a, b, c, d, segment_message[13], S21, 0xA9E3E905L);
	calculation (G, d, a, b, c, segment_message[ 2], S22, 0xFCEFA3F8L);
	calculation (G, c, d, a, b, segment_message[ 7], S23, 0x676F02D9L);
	calculation (G, b, c, d, a, segment_message[12], S24, 0x8D2A4C8AL);

	#define S31 4
	#define S32 11
	#define S33 16
	#define S34 23
	calculation (H, a, b, c, d, segment_message[ 5], S31, 0xFFFA3942L);
	calculation (H, d, a, b, c, segment_message[ 8], S32, 0x8771F681L);
	calculation (H, c, d, a, b, segment_message[11], S33, 0x6D9D6122L);
	calculation (H, b, c, d, a, segment_message[14], S34, 0xFDE5380CL);
	calculation (H, a, b, c, d, segment_message[ 1], S31, 0xA4BEEA44L);
	calculation (H, d, a, b, c, segment_message[ 4], S32, 0x4BDECFA9L);
	calculation (H, c, d, a, b, segment_message[ 7], S33, 0xF6BB4B60L);
	calculation (H, b, c, d, a, segment_message[10], S34, 0xBEBFBC70L);
	calculation (H, a, b, c, d, segment_message[13], S31, 0x289B7EC6L);
	calculation (H, d, a, b, c, segment_message[ 0], S32, 0xEAA127FAL);
	calculation (H, c, d, a, b, segment_message[ 3], S33, 0xD4EF3085L);
	calculation (H, b, c, d, a, segment_message[ 6], S34, 0x04881D05L);
	calculation (H, a, b, c, d, segment_message[ 9], S31, 0xD9D4D039L);
	calculation (H, d, a, b, c, segment_message[12], S32, 0xE6DB99E5L);
	calculation (H, c, d, a, b, segment_message[15], S33, 0x1FA27CF8L);
	calculation (H, b, c, d, a, segment_message[ 2], S34, 0xC4AC5665L);

	#define S41 6
	#define S42 10
	#define S43 15
	#define S44 21
	calculation (I, a, b, c, d, segment_message[ 0], S41, 0xF4292244L);
	calculation (I, d, a, b, c, segment_message[ 7], S42, 0x432AFF97L);
	calculation (I, c, d, a, b, segment_message[14], S43, 0xAB9423A7L);
	calculation (I, b, c, d, a, segment_message[ 5], S44, 0xFC93A039L);
	calculation (I, a, b, c, d, segment_message[12], S41, 0x655B59C3L);
	calculation (I, d, a, b, c, segment_message[ 3], S42, 0x8F0CCC92L);
	calculation (I, c, d, a, b, segment_message[10], S43, 0xFFEFF47DL);
	calculation (I, b, c, d, a, segment_message[ 1], S44, 0x85845DD1L);
	calculation (I, a, b, c, d, segment_message[ 8], S41, 0x6FA87E4FL);
	calculation (I, d, a, b, c, segment_message[15], S42, 0xFE2CE6E0L);
	calculation (I, c, d, a, b, segment_message[ 6], S43, 0xA3014314L);
	calculation (I, b, c, d, a, segment_message[13], S44, 0x4E0811A1L);
	calculation (I, a, b, c, d, segment_message[ 4], S41, 0xF7537E82L);
	calculation (I, d, a, b, c, segment_message[11], S42, 0xBD3AF235L);
	calculation (I, c, d, a, b, segment_message[ 2], S43, 0x2AD7D2BBL);
	calculation (I, b, c, d, a, segment_message[ 9], S44, 0xEB86D391L);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

unsigned long inv(unsigned long value)
{
    unsigned long res = 0;

    res |= ((value >> 0) & 0xFF) << 24;
    res |= ((value >> 8) & 0xFF) << 16;
    res |= ((value >> 16) & 0xFF) << 8;
    res |= ((value >> 24) & 0xFF) << 0;

    return res;
}

const char* getHash(vector<unsigned char> inputContent)
{
	alignment(inputContent);
	// инициализация буфера
	unsigned long* buf = new unsigned long[4];
	buf[0] = 0x67452301;
	buf[1] = 0xEFCDAB89;
	buf[2] = 0x98BADCFE;
	buf[3] = 0x10325476;

	for (int i = 0; i < inputContent.size() / (16 * 4); i++)//номер блока
	{
		unsigned long segment_message[16];
		for (int j = 0; j < 16; j++)//номер элемента в блоке
		{
			unsigned long mi = 0;
			for (int k = 0; k < 4; k++)
			{
				mi |= inputContent[i * 16 * 4 + j * 4 + k] << k * 8;
			}
			segment_message[j] = mi;
		}
		Rounds(buf, segment_message);
	}
	
	ostringstream res;
	res.fill ('0'); //нулями будут «залиты» пустые позиции в результирующей строке
	for (int i = 0; i < 4; i++)
	{
		res << std::hex << setw(8) << inv(buf[i]);
	}

	char* result = new char[33];
	strcpy(result, (char*) res.str().c_str());
	return result;
}