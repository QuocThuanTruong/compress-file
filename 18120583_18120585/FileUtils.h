#ifndef _FILE_UTILS
#define _FILE_UTILS

/**
 *	LZSS
 *	@author	Quoc Thuan Truong		18120583
 *	@since 2019-12-12
 */

#include <vector>
#include "dirent.h"
#include "LZSS.h"
using namespace std;

#pragma warning(disable: 4996)

#define MAX_LENGTH_OF_PATH 1024
#define MAX_FILE_NAME 256

typedef void (*ENCODE_DECODE_CALLBACK)(FILE*, FILE*);	/* Con trỏ hàm nén hoặc giải nén dữ liệu với 2 tham số là file đầu vào/đẩu ra */

class FileUtils {
private:
	struct DIRECTORY {
		int type;								/* Loại directory*/
		char prefix[MAX_LENGTH_OF_PATH];		/* Tiền tố của đường dẫn đến directory VD: .\\a\\b\\c.txt -> prefix: .\\a\\b */
		char name[MAX_FILE_NAME];				/* Tên của directory */
	};

	vector<DIRECTORY> listDirName;
	FILE* inFile;
	FILE* outFile;
private:
	vector<char*> seperateTokenFromPath(char* path);
	void listDirectory(char* basePath, int root, vector<DIRECTORY>& listDirName);
	int getTypeOfFile(char* fileName);
public:
	int checkPathOfFile(char* path);
	int checkPathOfFolder(char* path);
	void drawDirectoryTree(char* basePath);
	void compress(ENCODE_DECODE_CALLBACK encodeCallBack, char* srcPath, char* destPath);
	void decompress(ENCODE_DECODE_CALLBACK decodeCallBack, char* srcPath, char* destPath);
};

#endif // !_FILE_UTILS
