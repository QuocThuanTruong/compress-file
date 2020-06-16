#include "LZSS.h"

/**
 *	Hàm findMatchString - tìm chuỗi trùng nhau dài nhất (tối đa 18) giữa search buffer và lookahead buffer.
 *	@param		int					vị trí hiện tại trong lookahead buffer.
 *	@return		ENCODED_STRING		cấu trúc mã hóa bao gồm { offfset, length } - khoảng cách giữa 2 bộ đệm, chiều dài khớp.
 */

LZSS::ENCODED_STRING LZSS::findMatchString(int idxOfLookahead) {
	ENCODED_STRING matchData;
	int i, j;

	matchData.length = 0;
	i = lists[lookAheadBuffer[idxOfLookahead]]; 

	while (i != NULL_INDEX) {

		j = 1;

		while (searchBuffer[(i + j) % WINDOW_SIZE] == lookAheadBuffer[(idxOfLookahead + j) % MAX_CODED]) {
			if (j >= MAX_CODED) {
				break;
			}
			j++;
		};

		if (j > matchData.length && i < 4096) {
			matchData.length = j;
			matchData.offset = i;
		}

		if (j >= MAX_CODED)
		{
			matchData.length = MAX_CODED;
			break;
		}

		i = nextOfList[i];    /* tiếp tục với vị trí tiếp theo */
	}

	return matchData;
}

/**
 *	Hàm addChar - thêm vị trí đầu tiên của chuỗi trùng nhau vào danh sách, nếu đụng độ sẽ xử lí bằng phương pháp nối kết.
 *	@param		int		vị trí của kí tự đầu tiên trong chuỗi trùng.
 *	@return		NULL
 */
void LZSS::addChar(int charIndex) {
	int i;

	nextOfList[charIndex] = NULL_INDEX;

	if (lists[searchBuffer[charIndex]] == NULL_INDEX) {
		/* danh sách chưa có kí tự cần thêm */
		lists[searchBuffer[charIndex]] = charIndex;
		return;
	}

	/* tìm vị trí cuối cùng của danh sách để thêm vào */
	i = lists[searchBuffer[charIndex]];
	while (nextOfList[i] != NULL_INDEX)
	{
		i = nextOfList[i];
	}

	/* thêm kí tự vào danh sách đụng độ */
	nextOfList[i] = charIndex;
}

/**
 *	Hàm RemoveChar - xóa dữ liệu của charIndex đã thêm vào danh sách.
 *	@param		int		vị trí của kí tự đầu tiên trong chuỗi trùng cần xóa.
 *	@return		NULL
 */
void LZSS::RemoveChar(int charIndex) {
	int i;
	int nextIndex;

	nextIndex = nextOfList[charIndex];        /* lưu vị trí kí tự kế tiếp */
	nextOfList[charIndex] = NULL_INDEX;

	if (lists[searchBuffer[charIndex]] == charIndex) {
		/* danh sách chỉ có 1 kí tự */
		lists[searchBuffer[charIndex]] = nextIndex;
		return;
	}

	/* tìm vị trí cần xóa */
	i = lists[searchBuffer[charIndex]];
	while (nextOfList[i] != charIndex)
	{
		i = nextOfList[i];
	}

	/* trỏ đến kí tự tiếp theo */
	nextOfList[i] = nextIndex;
}

/**
 *	Hàm encodeLZSS - mã hóa inFile theo thuật toán LZSS (cải tiến với bảng băm) và xuất dữ liệu đã mã hóa ra outFile.
 *	@param		FILE*	  file đầu vào.
 *	@param		FILE*	  file đầu ra.
 *	@return		NULL
 */
void LZSS::encodeLZSS(FILE* inFile, FILE* outFile) {
	unsigned char bitFlags, flagPos, encodedData[16];
	int idxEncodedData;                
	ENCODED_STRING matchData;
	int i, c;
	int len;		/* Chiều dài dữ liệu trong lookahead buffer */
	int idxOfWindow, idxOfLookahead; 

	bitFlags = 0;
	flagPos = 0x01;
	idxEncodedData = 0;
	idxOfWindow = 0;
	idxOfLookahead = 0;

	/* Đưa MAX_CODED dữ liệu vào lookahead buffer */
	for (len = 0; len < MAX_CODED && (c = getc(inFile)) != EOF; len++) {
		lookAheadBuffer[len] = c;
	}

	if (len == 0) {
		return;  /* inFile rỗng */
	}

	/* Khởi tạo cửa sổ trượt, danh sách kí tự và danh sách đụng độ */
	for (i = 0; i < WINDOW_SIZE; i++) {
		searchBuffer[i] = ' ';
		nextOfList[i] = i + 1;
	}

	nextOfList[WINDOW_SIZE - 1] = NULL_INDEX;

	for (i = 0; i < 256; i++)
	{
		lists[i] = NULL_INDEX;
	}

	lists[' '] = 0;

	matchData = findMatchString(idxOfLookahead);
	
	/* Mã hóa dữ liệu đến hết file */
	while (len > 0) {
		if (matchData.length > len) { 
			matchData.length = len;
		}

		if (matchData.length <= MIN_CODED) {
			/* Dữ liệu trùng không đủ chiều dài tối thiểu */
			matchData.length = 1;   
			bitFlags |= flagPos;       /* Đánh dấu bit 1 rằng dữ liệu chưa được mã hóa*/
			encodedData[idxEncodedData++] = lookAheadBuffer[idxOfLookahead];
		}
		else {
			/* Mã hóa offset và length */
			encodedData[idxEncodedData++] =
				(unsigned char)((matchData.offset & 0x0FFF) >> 4);

			encodedData[idxEncodedData++] =
				(unsigned char)(((matchData.offset & 0x000F) << 4) |
				(matchData.length - (MIN_CODED + 1)));
		}

		if (flagPos == 0x80) {
			/* Nếu đủ 8 bit bitFlags thì ghi dữ liệu ra file */
			putc(bitFlags, outFile);

			for (i = 0; i < idxEncodedData; i++) {
				putc(encodedData[i], outFile);
			}

			/* Reset dữ liệu của các biến */
			bitFlags = 0;
			flagPos = 0x01;
			idxEncodedData = 0;
		}
		else {
			/* Dịch bit để đánh dấu dữ liệu tiếp theo */
			flagPos <<= 1;
		}

		/* Trượt cửa sổ length đơn vị khi đã tìm được chuỗi trùng */
		i = 0;
		while ((i < matchData.length) && ((c = getc(inFile)) != EOF)) {
			RemoveChar(idxOfWindow);
			searchBuffer[idxOfWindow] = lookAheadBuffer[idxOfLookahead];
			lookAheadBuffer[idxOfLookahead] = c;
			addChar(idxOfWindow);
			idxOfWindow = (idxOfWindow + 1) % WINDOW_SIZE;
			idxOfLookahead = (idxOfLookahead + 1) % MAX_CODED;
			i++;
		}

		/* xử lí khi đã cạn dữ liệu từ file mà chưa làm đầy lookahead buffer */
		while (i < matchData.length) {
			RemoveChar(idxOfWindow);
			searchBuffer[idxOfWindow] = lookAheadBuffer[idxOfLookahead];
			addChar(idxOfWindow);
			idxOfWindow = (idxOfWindow + 1) % WINDOW_SIZE;
			idxOfLookahead = (idxOfLookahead + 1) % MAX_CODED;
			len--;
			i++;
		}
		
		/* Tiếp tục tìm kiếm chuỗi trùng */
		matchData = findMatchString(idxOfLookahead);
	}

	/* Ghi phần dữ liệu thừa */
	if (idxEncodedData != 0) {
		putc(bitFlags, outFile);

		for (i = 0; i < idxEncodedData; i++) {
			putc(encodedData[i], outFile);
		}
	}
}

/**
 *	Hàm decodeLZSS - giải inFile theo thuật toán LZSS "ngược" và xuất dữ liệu đã giải mã ra outFile.
 *	@param		FILE*	  file đầu vào.
 *	@param		FILE*	  file đầu ra.
 *	@return		NULL
 */
void LZSS::decodeLZSS(FILE* inFile, FILE* outFile) {
	int  i, c;
	unsigned char bitFlags, flagsUsed;    
	int idxOfWindow;                       
	ENCODED_STRING code;             

	bitFlags = 0;
	flagsUsed = 7;
	idxOfWindow = 0;

	for (i = 0; i < WINDOW_SIZE; i++) {
		searchBuffer[i] = ' ';
	}

	while (true) {
		bitFlags >>= 1;
		flagsUsed++;

		if (flagsUsed == 8) {
			/* Nếu bit đã sử dụng là 8, tiếp tục trượt để đọc 8 bit tiếp theo */
			if ((c = getc(inFile)) == EOF) {
				break;
			}

			bitFlags = c & 0xFF;
			flagsUsed = 0;
		}

		if (bitFlags & 0x01) {
			/* Nếu là kí tự chưa mã hóa (bit 1) */
			if ((c = getc(inFile)) == EOF) {
				break;
			}

			putc(c, outFile);
			searchBuffer[idxOfWindow] = c;
			idxOfWindow = (idxOfWindow + 1) % WINDOW_SIZE;
		}
		else {
			/* Đọc và giải mã offset và length */
			if ((code.offset = getc(inFile)) == EOF) {
				break;
			}

			if ((code.length = getc(inFile)) == EOF) {
				break;
			}

			code.offset <<= 4;
			code.offset |= ((code.length & 0x00F0) >> 4);
			code.length = (code.length & 0x000F) + MIN_CODED + 1;

			for (i = 0; i < code.length; i++) {
				c = searchBuffer[(code.offset + i) % WINDOW_SIZE];
				putc(c, outFile);
				lookAheadBuffer[i] = c;
			}

			for (i = 0; i < code.length; i++) {
				searchBuffer[(idxOfWindow + i) % WINDOW_SIZE] = lookAheadBuffer[i];
			}

			idxOfWindow = (idxOfWindow + code.length) % WINDOW_SIZE;
		}
	}
}

/**
 *	Contructor LZSS - khởi tạo các giá trị cho các biến trong lớp.
 *	@param		int		kích thước cửa sổ trượt, mặc định 4096 (nên thiết đặt là 4096 hoặc lớn hơn).
 *	@return		NULL
 */
LZSS::LZSS(unsigned int WINDOW_SIZE) {
	this->WINDOW_SIZE = WINDOW_SIZE;
	searchBuffer = new unsigned char[WINDOW_SIZE];
	lookAheadBuffer = new unsigned char[MAX_CODED];
	nextOfList = new unsigned int[WINDOW_SIZE];
}

/**
 *	Destructor LZSS - giải phóng các vùng nhớ đã cấp phát.
 *	@param		NULL
 *	@return		NULL
 */
LZSS::~LZSS(){
	if (searchBuffer) {
		delete[] searchBuffer;
	}

	if (lookAheadBuffer) {
		delete[] lookAheadBuffer;
	}

	if (nextOfList) {
		delete[] nextOfList;
	}
}
