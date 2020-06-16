#include "FileUtils.h"

/**
 *	Hàm seperateTokenFromPath - tách đường dẫn ban đầu thành các token nhỏ hơn gồm tên thư mục và tập tin.
 *	@param		char*				đường dẫn cần tách thành các tokens.
 *	@return		vector<char*>		mảng các tokens đã được tách.
 */
	vector<char*> FileUtils::seperateTokenFromPath(char* path) {
	vector<char*> result;
	char* temp, * token;
	char sep[3] = "\\/";

	temp = new char[strlen(path) + 1];
	strcpy(temp, path);

	/* Bỏ vị trí thư mục gốc */
	while (temp[0] != '\\') {
		for (int i = 0; i < strlen(temp); i++) {
			temp[i] = temp[i + 1];
		}
		temp[strlen(temp)] = '\0';
	}

	token = strtok(temp, sep);

	while (token) {
		result.push_back(token);
		token = strtok(NULL, sep);
	}

	return result;
}

/**
 *	Hàm listDirectory - vẽ cây thư mục ra màn hình, đồng thời liệt kê tất các các thư mục con và tập tin.
 *	@param		char*					đường dẫn ban đầu.
 *	@param		int						vị trí thực hiện quay lui (mặc định là 0).
 *	@param		vector<DIRECTORY>&		danh sách lưu các thư mục và tập tin đã liệt kê - lưu theo cấu trúc DIRECTORY đã định nghĩa trước.
 *	@return		NULL
 */
void FileUtils::listDirectory(char* basePath, int root, vector<DIRECTORY>& listDirName) {
	char path[MAX_LENGTH_OF_PATH];
	dirent* dp;
	DIR* dir;

	dir = opendir(basePath);

	if (!dir) {
		return;
	}

	while ((dp = readdir(dir)) != NULL) {
		DIRECTORY dr;

		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
			for (int i = 0; i < root; i++) {
				if (i % 2 == 0 || i == 0) {
					printf("%c", 179);
				}
				else {
					printf("  ");
				}
			}

			printf("%c%c%s\n", 195, 196, dp->d_name);
			/* Lưu các directory */
			dr.type = dp->d_type;
			strcpy(dr.name, dp->d_name);
			strcpy(dr.prefix, basePath);
			listDirName.push_back(dr);

			strcpy(path, basePath);
			strcat(path, "\\");
			strcat(path, dp->d_name);
			/* Quay lui */
			listDirectory(path, root + 2, listDirName);
		}
	}

	closedir(dir);
}

/**
 *	Hàm getTypeOfFile - tìm loại của tập tin.
 *	@param		char*		tên tập tin.
 *	@return		int			loại tập tin DT_REG - tập tin thường, DT_DIR - thư mục.
 */
int FileUtils::getTypeOfFile(char* fileName) {
	for (int i = 0; i < listDirName.size(); i++) {
		if (strcmp(fileName, listDirName[i].name) == 0) {
			return listDirName[i].type;
		}
	}

	return 0;
}

/**
 *	Hàm checkPathOfFile - kiểm tra đường dẫn đưa vào có phải trỏ đến 1 tập tin hay không.
 *	@param		char*		tên đường dẫn.
 *	@return		int			0 - nếu không phải là tập tin, 1 - đường dẫn trỏ đến tập tin.
 */
int FileUtils::checkPathOfFile(char* path) {
	FILE* testFile;

	testFile = fopen(path, "rb");
	if (testFile == NULL) {
		return 0;
	}
	fclose(testFile);

	return 1;
}

/**
 *	Hàm checkPathOfFile - kiểm tra đường dẫn đưa vào có phải trỏ đến 1 thư mục hay không.
 *	@param		char*		tên đường dẫn.
 *	@return		int			0 - nếu không phải là thư mục, 1 - đường dẫn trỏ đến thư mục.
 */
int FileUtils::checkPathOfFolder(char* path) {
	DIR* dir;

	dir = opendir(path);
	if (dir == NULL) {
		return 0;
	}

	return 1;
}

/**
 *	Hàm drawDirectoryTree - hàm bọc của hàm listDirectory.
 *	@param		char*		tên đường dẫn cần vẽ và liệt kê.
 *	@return		NULL
 */
void FileUtils::drawDirectoryTree(char* basePath) {
	listDirectory(basePath, 0, listDirName);
}

/**
 *	Hàm compress - thực hiện việc đọc các đường dẫn, và nén chúng theo cấu trúc đã quy ước trong file báo cáo.
 *	@param		ENCODE_DECODE_CALLBACK		con trỏ hàm nén dữ liệu của thuật toán bất kì được tổ chức hàm theo định nghĩa ENCODE_DECODE_CALLBACK.
 *	@return		NULL
 */
void FileUtils::compress(ENCODE_DECODE_CALLBACK encodeCallBack, char* srcPath, char* destPath) {
	DIR* dir;
	char outfileName[MAX_FILE_NAME] = "";

	dir = opendir(srcPath);

	if (dir != NULL) {
		strcat(outfileName, srcPath);
	}
	else { /* Đường dẫn trỏ đến 1 tập tin cần nén */
		inFile = fopen(srcPath, "rb");
		outFile = fopen(destPath, "wb+");

		int* len;
		
		/* Ghi 3 byte đánh dấu */
		fprintf(outFile, "%c", 0xFF);
		fprintf(outFile, "%c", 0xF);
		fprintf(outFile, "%c", 0x0);

		/* Ghi tên file */
		len = new int;
		*len = (int)strlen(srcPath);
		fwrite(len, 4, 1, outFile);
		fprintf(outFile, srcPath);

		/* Nén dữ liệu */
		encodeCallBack(inFile, outFile);

		fcloseall();
		return;
	}

	strcat(outfileName, "_temp.txt");
	remove(outfileName);
	outFile = fopen(outfileName, "ab+");

	for (int i = 0; i < listDirName.size(); i++) {
		char infileName[MAX_FILE_NAME] = "";

		strcat(infileName, listDirName[i].prefix);
		strcat(infileName, "\\");
		strcat(infileName, listDirName[i].name);

		/* Nếu là tập tin */
		if (listDirName[i].type == DT_REG) {
			int* len, * size;
			int _c;

			inFile = fopen(infileName, "rb");

			/* Ghi 2 byte đánh dấu */
			fprintf(outFile, "%c", 0xFF);
			fprintf(outFile, "%c", 0xF);

			/* Ghi tên file */
			len = new int;
			*len = (int)strlen(infileName);
			fwrite(len, 4, 1, outFile);
			fprintf(outFile, infileName);

			/* Ghi kích thước file */
			fseek(inFile, 0, SEEK_END);
			size = new int;
			*size = ftell(inFile);
			fseek(inFile, 0, SEEK_SET);
			fwrite(size, 4, 1, outFile);

			/* Ghi data của file */
			while ((_c = fgetc(inFile)) != EOF) {
				putc(_c, outFile);
			}

			fclose(inFile);
		}
		else {	/* Nếu là thư mục */
			int* len, * size;
			/* Ghi 2 byte đánh dấu */
			fprintf(outFile, "%c", 0xFF);
			fprintf(outFile, "%c", 0xF);

			/* Ghi tên file */
			len = new int;
			*len = (int)strlen(infileName);
			fwrite(len, 4, 1, outFile);
			fprintf(outFile, infileName);

			/* Ghi kích thước file */
			size = new int;
			*size = 0;
			fwrite(size, 4, 1, outFile);
		}
	}
	fcloseall();

	/* Nén dữ liệu */
	inFile = fopen(outfileName, "rb");
	outFile = fopen(destPath, "wb");
	encodeCallBack(inFile, outFile);

	fcloseall();
	remove(outfileName);
}

/**
 *	Hàm decompress - thực hiện việc nén dữ liệu, tổ chức lại các cấu trúc ban đầu đã nén.
 *	@param		ENCODE_DECODE_CALLBACK		con trỏ hàm giải nén dữ liệu của thuật toán bất kì được tổ chức hàm theo định nghĩa ENCODE_DECODE_CALLBACK.
 *	@return		NULL
 */
void FileUtils::decompress(ENCODE_DECODE_CALLBACK decodeCallBack, char* srcPath, char* destPath) {
	DIR* dir;
	int decomSize;
	char outfileName[MAX_FILE_NAME] = "_temp.txt";

	inFile = fopen(srcPath, "rb");
	remove(outfileName);

	/* Kiểm tra 3 byte nhận dạng nếu là của tập tin thì tiến hành giải nén ngay */
	//char* reg_file = new char[3];
	//fread(reg_file, 3, 1, inFile);

	//if (reg_file[0] == 0xFFFFFFFF && reg_file[1] == 0xF && reg_file[2] == 0x0) {
	//	int* len;
	//	char* pathDir;
	//	vector<char*> tokens;

	//	/* Đọc đường dẫn của tập tin*/
	//	len = new int;
	//	fread(len, 4, 1, inFile);

	//	pathDir = new char[*len + 1];

	//	for (int i = 0; i < *len; i++) {
	//		pathDir[i] = fgetc(inFile);
	//	}
	//	pathDir[*len] = '\0';

	//	/* Tạo đường dẫn cho tập tin */
	//	tokens = seperateTokenFromPath(pathDir);

	//	char* newDir = new char[strlen(destPath) + 1];
	//	strcpy(newDir, destPath);
	//	char cmdMakeDir[1000] = "mkdir ";
	//	strcat(cmdMakeDir, "\"");
	//	strcat(cmdMakeDir, newDir);
	//	strcat(cmdMakeDir, "\"");

	//	dir = opendir(newDir);
	//	if (dir == NULL) {
	//		system(cmdMakeDir);
	//	}
	//	closedir(dir);

	//	strcat(newDir, "\\");
	//	strcat(newDir, tokens[tokens.size() - 1]);

	//	outFile = fopen(newDir, "wb");

	//	/* Giải nén */
	//	decodeCallBack(inFile, outFile);

	//	fclose(outFile);
	//	return;
	//}

	/* Giải nén thư mục */
	//fseek(inFile, 0, SEEK_SET);
	outFile = fopen(outfileName, "wb+");
	decodeCallBack(inFile, outFile);

	decomSize = ftell(outFile);
	fclose(inFile);
	fseek(outFile, 0, SEEK_SET);

	while (ftell(outFile) != decomSize) {
		char* reg;
		int* len, * fileSize;
		char* pathDir;
		vector<char*> tokens;

		/* Đọc các byte đánh dấu và kiểm tra*/
		reg = new char[2];
		fread(reg, 2, 1, outFile);

		if (reg[0] != 0xFF || reg[1] != 0xF) {
			printf("Can not decompress because file format is invalid!\n");
			return;
		}

		/* Đọc tên tập tin, thư mục */
		len = new int;
		fread(len, 4, 1, outFile);

		pathDir = new char[*len + 1];

		for (int i = 0; i < *len; i++) {
			pathDir[i] = fgetc(outFile);
		}
		pathDir[*len] = '\0';

		/* Tạo đường dẫn cho tập tin, thư mục*/
		tokens = seperateTokenFromPath(pathDir);

		char* newDir = new char[strlen(destPath) + 1];
		strcpy(newDir, destPath);
		char cmdMakeDir[1000] = "mkdir ";
		strcat(cmdMakeDir, "\"");
		strcat(cmdMakeDir, newDir);
		strcat(cmdMakeDir, "\"");

		dir = opendir(newDir);
		if (dir == NULL) {
			system(cmdMakeDir);
		}
		closedir(dir);

		/* Tạo tất cả các đường dẫn */
		for (int i = 0; i < tokens.size() - 1; i++) {
			strcat(cmdMakeDir, "\\");
			strcat(cmdMakeDir, "\"");
			strcat(newDir, "\\");

			strcat(cmdMakeDir, tokens[i]);
			strcat(cmdMakeDir, "\"");
			strcat(newDir, tokens[i]);

			dir = opendir(newDir);
			if (dir == NULL) {
				system(cmdMakeDir);
			}
			closedir(dir);
		}

		strcat(newDir, "\\");
		strcat(newDir, tokens[tokens.size() - 1]);

		/* Nếu là thư mục thì tạo */
		if (getTypeOfFile(tokens[tokens.size() - 1]) == DT_DIR) {
			strcat(cmdMakeDir, "\\");
			strcat(cmdMakeDir, "\"");
			strcat(cmdMakeDir, tokens[tokens.size() - 1]);
			strcat(cmdMakeDir, "\"");

			dir = opendir(newDir);
			if (dir == NULL) {
				system(cmdMakeDir);
			}
			closedir(dir);
		}

		/* Đọc kích thước file */
		fileSize = new int;
		fread(fileSize, 4, 1, outFile);

		/* Nếu là tập tin thì tạo tập tin và ghi dữ liệu */
		if (getTypeOfFile(tokens[tokens.size() - 1]) == DT_REG) {
			FILE* newFile = fopen(newDir, "wb");

			for (int i = 0; i < *fileSize; i++) {
				putc(fgetc(outFile), newFile);
			}

			fclose(newFile);
		}
	}

	fcloseall();
	remove(outfileName);
}
