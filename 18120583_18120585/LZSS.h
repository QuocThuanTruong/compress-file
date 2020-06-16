#ifndef _LZSS_H
#define _LZSS_H

/**
 *	LZSS
 *	@author	Quoc Thuan Truong		18120583
 *	@since 2019-12-12
 */

#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable: 4996)

class LZSS {
private:
	unsigned int WINDOW_SIZE;
	unsigned int NULL_INDEX = (WINDOW_SIZE + 1);
	const int MIN_CODED = 2;
	const int MAX_CODED = 15 + MIN_CODED + 1;
private:
	struct ENCODED_STRING {
		int offset;			/* Khoảng cách giữ các kí tự trùng nhau */
		int length;			/* Chiều dài trùng nhau */
	};

	/* Sliding window */
	unsigned char* searchBuffer;
	unsigned char* lookAheadBuffer;

	unsigned int lists[256];
	unsigned int* nextOfList;             /* Danh sách xử lí đụng độ */
private:
	ENCODED_STRING findMatchString(int idxOfLookahead);
	void addChar(int charIndex);
	void RemoveChar(int charIndex);
public:
	void encodeLZSS(FILE* inFile, FILE* outFile);
	void decodeLZSS(FILE* inFile, FILE* outFile);
public:
	LZSS(unsigned int WINDOW_SIZE = 4096);
	~LZSS();
};

#endif // !_LZSS_H
