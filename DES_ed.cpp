#include <iostream>
#include <algorithm>
#include <bitset>
using namespace std;

/***  DES: Initial/Final Permutation  ***/
int IP[64] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};
int IPinv[64] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41,  9, 49, 17, 57, 25
};
/****************************************/

/** Feistel Function: Expansion P-box ***/
int Expbox[48] = {
	32,  1,  2,  3,  4,  5,
	 4,  5,  6,  7,  8,  9,
	 8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1
};

/****** Feistel Function: S-boxes *******/
int Sbox[8][4][16] = {
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};

/*** Feistel Function: Straight P-box ***/
int Strabox[32] = {
	16,  7, 20, 21, 29, 12, 28, 17,
	 1, 15, 23, 26,  5, 18, 31, 10,
	 2,  8, 24, 14, 32, 27,  3,  9,
	19, 13, 30,  6, 22, 11,  4, 25
};
/****************************************/

/******* Key: Left Shift Schedule *******/
int KeyShift[16] = {
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

/******* Key: Permuted Choice One *******/
int PC_1[56] = {
	57, 49, 41, 33, 25, 17,  9,
	 1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	 7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};

/******* Key: Permuted Choice Two *******/
int PC_2[56] = {
	14, 17, 11, 24,  1,  5,
	 3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};
/****************************************/

bitset<64> ASCII_2_Binary(string s) {
	bitset<64> res;
	for (int i = 0; i < 8; ++i) {
		int ch = s[i];
		for (int j = 7; j >= 0; --j) {
			res[8 * i + j] = ch % 2;
			ch /= 2;
		}
	}
	return res;
}

string Binary_2_Hex(bitset<64> n) {
	string res;
	res.resize(16);
	for (int i = 0; i < 16; ++i) {
		int ch = 0;
		// The closer, the more significant.
		for (int j = 0; j < 4; ++j) {
			ch += n[4 * i + j] * (1 << (3 - j));
		}
		res[i] = (ch < 10 ? char('0' + ch) : char('A' + ch - 10));
	}
	return res;
}

bitset<28> LeftCircShift(bitset<28> x, int n) {
	return (x >> n) | (x << (x.size() - n));
}

bitset<32> Feistel(bitset<32> R, bitset<48> K) {
	// 32-bit expands to 48-bit
	bitset<48> R_Exp;
	for (int i = 0; i < 48; ++i) {
		R_Exp[i] = R[ Expbox[i] - 1 ];
	}
	// XOR with K
	R_Exp = R_Exp ^ K;
	// 6-bit a group -> 8 4-bit numbers
	bitset<32> tmp;
	for (int i = 0; i < 8; ++i) {
		int row = 2 * R_Exp[6*i]     + 1 * R_Exp[6*i + 5];
		int col = 8 * R_Exp[6*i + 1] + 4 * R_Exp[6*i + 2] + 2 * R_Exp[6*i + 3] + 1 * R_Exp[6*i + 4];
		int val = Sbox[i][row][col];
		for (int j = 3; j >= 0; --j) {
			tmp[4 * i + j] = val % 2;
			val /= 2;
		}
	}
	// 32-bit to 32-bit
	bitset<32> res;
	for (int i = 0; i < 32; ++i) {
		res[i] = tmp[ Strabox[i] - 1 ];
	}
	return res;
}

string DES_en(string key, string plaintext) {
	bitset<64> nPlaintext = ASCII_2_Binary(plaintext);
	bitset<64> nKey = ASCII_2_Binary(key);
	bitset<64> tmp;
//	cout << nKey << '\n' << nPlaintext << '\n';

	// initial permutation
	for (int i = 0; i < 64; ++i) {
		tmp[i] = nPlaintext[ IP[i] - 1 ];
	}
	for (int i = 0; i < 64; ++i) {
		nPlaintext[i] = tmp[i];
	}
	// split to L/R (32-bit)
	bitset<32> L, R;
	for (int i = 0; i < 32; ++i) {
		L[i] = nPlaintext[i];
		R[i] = nPlaintext[32 + i];
	}
	// key PC-1 (64-bit to 56 bit)
	for (int i = 0; i < 56; ++i) {
		tmp[i] = nKey[ PC_1[i] - 1 ];
	}
	// split to L/R (28-bit)
	bitset<28> nKeyL, nKeyR;
	for (int i = 0; i < 28; ++i) {
		nKeyL[i] = tmp[i];
		nKeyR[i] = tmp[28 + i];
	}
	// 16 rounds f funtion
	for (int round = 0; round < 16; ++round) {
		// left circular shift
		nKeyL = LeftCircShift(nKeyL, KeyShift[round]);
		nKeyR = LeftCircShift(nKeyR, KeyShift[round]);
		// key PC-2
		bitset<48> K;
		for (int i = 0; i < 48; ++i) {
			K[i] = (PC_2[i] <= 28 ? nKeyL[ PC_2[i] - 1 ] : nKeyR[ PC_2[i] - 29 ]);
		}
		// f
		bitset<32> res = Feistel(R, K);
		bitset<32> jiz = R;
		R = L ^ res;
		L = jiz;
	}
	// 32-bit swap and inv permutation
	for (int i = 0; i < 64; ++i) {
		tmp[i] = (IPinv[i] <= 32 ? R[ IPinv[i] - 1 ] : L[ IPinv[i] - 33 ]);
	}
//	cout << tmp << '\n';
	return Binary_2_Hex(tmp);
}

void DES_de(void key, void ciphertext) {
	// IP
	// L R
	for (int round = 15; round >= 0; --round) {
		bitset<32> res = Feistel(R, K[round]);
		bitset<32> jiz = R;
		R = L ^ res;
		L = jiz;
	}
	// 32-bit swap
	// inv IP
}

signed main() {
	ios_base::sync_with_stdio(false), cin.tie(0);
	int t = 5;
	while (t--) {
		string key, plaintext;
		cin >> key >> plaintext;
		string ciphertext = DES_en(key, plaintext);
		cout << ciphertext << '\n';
	}
	return 0;
}