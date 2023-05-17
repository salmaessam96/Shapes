#include<string>
#include <iostream>
using namespace std;

class DES {
public:
	string *splittxt;
	string hex[16][2] = {
		{"0", "0000"}, {"1", "0001"}, {"2", "0010"}, {"3", "0011"}, {"4", "0100"},
		{"5", "0101"}, {"6", "0110"}, {"7", "0111"}, {"8", "1000"}, {"9", "1001"},
		{"A", "1010"}, {"B", "1011"}, {"C", "1100"}, {"D", "1101"}, {"E", "1110"},
		{"F", "1111"}
	};
	int IP[64] = {
		58, 50, 42, 34, 26, 18, 10, 2,
		60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6,
		64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17, 9, 1,
		59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5,
		63, 55, 47, 39, 31, 23, 15, 7
	};
	int FP[64] = {
		40, 8, 48, 16, 56, 24, 64, 32,
		39, 7, 47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30,
		37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28,
		35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26,
		33, 1, 41, 9,  49, 17, 57, 25
	};
	int EXP[48] = {
		32, 1, 2, 3, 4, 5,
		4, 5, 6, 7, 8, 9,
		8, 9, 10, 11, 12, 13,
		12, 13,	14, 15, 16, 17,
		16, 17,	18, 19, 20, 21,
		20, 21,	22, 23, 24, 25,
		24, 25,	26, 27, 28, 29,
		28, 29,	30, 31, 32, 1
	};
	int Sboxes[8][4][16] = {
		{ //S1
			14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
			0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
			4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
			15,	12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
		},
		{ //S2
			15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
			3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
			0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
			13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
		},
		{ //S3
			10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
			13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
			13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
			1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
		},
		{ //S4
			7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
			13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
			10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
			3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
		},
		{ //S5
			2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
			14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
			4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
			11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
		},
		{ //S6
			12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
			10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
			9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
			4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
		},
		{ //S7
			4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
			13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
			1, 4, 11, 13, 12, 3, 7,	14, 10, 15, 6, 8, 0, 5, 9, 2,
			6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
		},
		{ //S8
			13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
			1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
			7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
			2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
		}
	};
	int F[32] = {
		16, 7, 20, 21, 29, 12, 28, 17,
		1, 15, 23, 26, 5, 18, 31, 10,
		2, 8, 24, 14, 32, 27, 3, 9,
		19, 13, 30, 6, 22, 11, 4, 25
	};
	int PC1[56] = {
		57, 49, 41, 33, 25, 17, 9, 1,
		58, 50, 42, 34, 26, 18, 10, 2,
		59, 51, 43, 35, 27, 19, 11, 3,
		60, 52, 44, 36, 63, 55, 47, 39,
		31, 23, 15, 7, 62, 54, 46, 38,
		30, 22, 14, 6, 61, 53, 45, 37,
		29, 21, 13, 5, 28, 20, 12, 4
	};
	int PC2[48] = {
		14, 17, 11, 24, 1, 5, 3, 28,
		15, 6, 21, 10, 23, 19, 12, 4,
		26, 8, 16, 7, 27, 20, 13, 2,
		41, 52, 31, 37, 47, 55, 30, 40,
		51, 45, 33, 48, 44, 49, 39, 56,
		34, 53, 46, 42, 50, 36, 29, 32
	};

	string Permutation(int *a, int n, string txt) {
		string newtxt = "";
		for (int i = 0; i < n; i++) {
			newtxt = newtxt + txt[a[i] - 1];
		}
		return newtxt;
	}

	string Lshift(string txt)
	{
		string newtxt = "";
		for (int j = 1; j < 28; j++) {
			newtxt += txt[j];
		}
		newtxt += txt[0];
		return newtxt;
	}
	string Expand(string txt)
	{
		string newtxt = "";
		for (int i = 0; i < 48; i++)
		{
			newtxt += txt[EXP[i] - 1];
		}
		return newtxt;
	}
	string XOR(string x, string y)
	{
		int n = x.size();
		string newtxt = "";
		for (int i = 0; i < n; i++) {
			if (x[i] != y[i])
				newtxt += "1";
			else
				newtxt += "0";
		}
		return newtxt;
	}

	string checkbin(char s1, char s2, char s3, char s4) {
		string newtxt = "";
		switch (s1) {
		case '0':
			switch (s2) {
			case '0':
				if (s3 == '0')
					if (s4 == '0')
						newtxt = "0";
					else
						newtxt = "1";
				else
					if (s4 == '0')
						newtxt = "2";
					else
						newtxt = "3";
				break;
			case '1':
				if (s3 == '0')
					if (s4 == '0')
						newtxt = "4";
					else
						newtxt = "5";
				else
					if (s4 == '0')
						newtxt = "6";
					else
						newtxt = "7";
				break;
			}
			break;
		case '1':
			switch (s2) {
			case '0':
				if (s3 == '0')
					if (s4 == '0')
						newtxt = "8";
					else
						newtxt = "9";
				else
					if (s4 == '0')
						newtxt = "A";
					else
						newtxt = "B";
				break;
			case '1':
				if (s3 == '0')
					if (s4 == '0')
						newtxt = "C";
					else
						newtxt = "D";
				else
					if (s4 == '0')
						newtxt = "E";
					else
						newtxt = "F";
				break;
			}
		}
		return newtxt;
	}
	string convertBH(string txt) {
		int n = txt.size();
		string newtxt = "";
		for (int i = 0; i < n; i += 4) {
			newtxt += checkbin(txt[i], txt[i + 1], txt[i + 2], txt[i + 3]);
		}
		return newtxt;
	}
	string checkhex(char txt) {
		string newtxt = "";
		switch (txt) {
		case '0':
			newtxt = "0000";
			break;
		case '1':
			newtxt = "0001";
			break;
		case '2':
			newtxt = "0010";
			break;
		case '3':
			newtxt = "0011";
			break;
		case '4':
			newtxt = "0100";
			break;
		case '5':
			newtxt = "0101";
			break;
		case '6':
			newtxt = "0110";
			break;
		case '7':
			newtxt = "0111";
			break;
		case '8':
			newtxt = "1000";
			break;
		case '9':
			newtxt = "1001";
			break;
		case 'A':
			newtxt = "1010";
			break;
		case 'B':
			newtxt = "1011";
			break;
		case 'C':
			newtxt = "1100";
			break;
		case 'D':
			newtxt = "1101";
			break;
		case 'E':
			newtxt = "1110";
			break;
		case 'F':
			newtxt = "1111";
			break;
		}
		return newtxt;
	}
	string convertHB(string txt) {
		char newtxt = ' ';
		string result = "";
		int n = txt.size();
		for (int i = 0; i < n; i++) {
			newtxt = txt[i];
			result += checkhex(newtxt);
		}
		return  result;
	}

	string convertDH(string txt) {
		int m = txt.size();
		string newtxt = "", tmp;
		int n = 0;
		for (int i = 0; i < m; i++) {
			n = int(char(txt[i]));
			while (n != 0) {
				int rem = 0;
				char ch;
				rem = n % 16;
				if (rem < 10) {
					ch = rem + 48;
				}
				else {
					ch = rem + 55;
				}
				tmp += ch;
				n = n / 16;
			}
			int start = 0, end = tmp.size() - 1;
			while (start <= end)
			{
				swap(tmp[start], tmp[end]);
				start++;
				end--;
			}
			newtxt += tmp;
			tmp = "";
		}
		return newtxt;
	}
	string Encryption(string txt, string *S) {
		string left[17], right[17];
		txt = Permutation(IP, 64, txt);
		for (int i = 0; i < 32; i++) {
			left[0] += txt[i];
		}
		for (int i = 32; i < 64; i++) {
			right[0] += txt[i];
		}
		for (int i = 0; i < 16; i++) {
			string R = Permutation(EXP, 48, right[i]);
			string x = XOR(S[i], R);
			string y = "";
			for (int j = 0; j < 8; j++) {
				int r = 2 * int(x[j * 6] - '0') + int(x[j * 6 + 5] - '0');//-'0' 3shan matetla3sh hex
				int c = 8 * int(x[j * 6 + 1] - '0') + 4 * int(x[j * 6 + 2] - '0') + 2 * int(x[j * 6 + 3] - '0') + int(x[j * 6 + 4] - '0');
				int s = Sboxes[j][r][c];
				y += char(s / 8 + '0');
				s = s % 8;
				y += char(s / 4 + '0');
				s = s % 4;
				y += char(s / 2 + '0');
				s = s % 2;
				y += char(s + '0');
			}
			y = Permutation(F, 32, y);
			x = XOR(y, left[i]);
			right[i + 1] = x;
			left[i + 1] = right[i];
		}
		swap(left[16], right[16]);
		string combine = left[16] + right[16];
		string cipher = Permutation(FP, 64, combine);
		cipher = convertBH(cipher);
		return cipher;
	}
	int blocks(string txt) {
		string newtxt = "", combine = "";
		int reminder = txt.size() % 64;
		int n = txt.size() - reminder;
		int sub = 64 - reminder;
		if (reminder != 0) {
			for (int k = 0; k < sub; k++) {
				newtxt += "0";
			}
		}
		combine = txt + newtxt;
		int i = n / 64;
		splittxt = new string[i + 1];
		for (int j = 0; j < i + 1; j++) {
			newtxt = "";
			for (int k = 64 * j; k < 64 * (j + 1); k++) {
				newtxt += combine[k];
			}
			splittxt[j] = newtxt;
		}
		return (i + 1);
	}

	int decblocks(string txt) {
		string newtxt = "";
		int i = txt.size() / 64;
		splittxt = new string[i];
		for (int j = 0; j < i ; j++) {
			newtxt = "";
			for (int k = 64 * j; k < 64 * (j + 1); k++) {
				newtxt += txt[k];
			}
			splittxt[j] = newtxt;
		}
		return (i);
	}

	int convertHD(string txt)
	{
		int base = 1;
		int newtxt = 0;
		for (int i = txt.size() - 1; i >= 0; i--) {
			if (txt[i] >= '0' && txt[i] <= '9') {
				newtxt += (int(txt[i]) - 48) * base;
				base = base * 16;
			}
			else if (txt[i] >= 'A' && txt[i] <= 'F') {
				newtxt += (int(txt[i]) - 55) * base;
				base = base * 16;
			}
		}
		return newtxt;
	}

	string convertHA(string txt) {
		string newtxt = "",asci="";
		for (int i = 0; i < txt.size(); i+=2) {
			newtxt = txt.substr(i, 2);
			char a = convertHD(newtxt);
			asci += a;
		}
		cout << asci;
		return asci;
	}
};
int main()
{
	DES encrypt,decrypt;
	string txt;
	//= "I'm very pleased that the authors have  succeeded in creating a highly valuable introduction to the subject of applied cryptography. I hope that it can serve as a guide for practitioners to build more secure systems based on cryptography, and as a stepping stone for future researchers to explore the exciting world of cryptography and its applications.";
	//string txt = "salma";
	cin >> txt;
	cout << "Please enter a key of 8 characters: ";
	string key;
	cin >> key;
	while (key.size() !=8) {
		cout << "\nInvalid input. Please enter a key of 8 characters: ";
		cin>> key;
	}
	key = encrypt.convertDH(key);
	string newtxt = "", bin, C, D, K[16], DK[16], cipher = "";
	int x;
	key = encrypt.convertHB(key);
	key = encrypt.Permutation(encrypt.PC1, 56, key);
	for (int i = 0; i < 28; i++) {
		C += key[i];
	}
	for (int i = 28; i < 56; i++) {
		D += key[i];
	}
	for (int i = 0; i < 16; i++) {
		if (i == 0 || i == 1 || i == 8 || i == 15)//round 1,2,9,16 shift 1 bit
		{
			C = encrypt.Lshift(C);
			D = encrypt.Lshift(D);
		}
		else
		{
			C = encrypt.Lshift(C);
			D = encrypt.Lshift(D);
			C = encrypt.Lshift(C);
			D = encrypt.Lshift(D);
		}
		string combine = C + D;
		string newK = encrypt.Permutation(encrypt.PC2, 48, combine);
		K[i] = newK;
	}
	if (txt.size() != 8) {
		newtxt = encrypt.convertDH(txt);
		bin = encrypt.convertHB(newtxt);
		x = encrypt.blocks(bin);
		string encryptedtxt = "";
		for (int i = 0; i < x; i++) {
			encryptedtxt = encrypt.Encryption(encrypt.splittxt[i], K);
			cipher += encryptedtxt;
		}
	}
	else {
		newtxt = encrypt.convertDH(txt);//use if it's ascii characters
		bin = encrypt.convertHB(newtxt);//use parameter txt if it's in hex, newtxt if it's in ascii
		cipher = encrypt.Encryption(bin, K);
	}
	cout << "\nCipher Text: " << cipher << endl;


	//decryption

	for (int i = 0; i < 16; i++) {
		DK[15 - i] = K[i];//reversing the keys
	}
	bin = decrypt.convertHB(cipher);//from hex to binary
	string decryptedtxt = "", plain = "";
	if (cipher.size() > 16) {
		x = decrypt.decblocks(bin);//breakdown text to blocks
		for (int i = 0; i < x; i++) {
			plain = decrypt.Encryption(decrypt.splittxt[i], DK);
			decryptedtxt += plain;
		}
	}
	else {
		decryptedtxt = decrypt.Encryption(bin, DK);
	}
	cout << "\nPlain Text: ";
	decrypt.convertHA(decryptedtxt);
}