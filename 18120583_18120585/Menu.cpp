#include "Menu.h"
#include "Huffman.h"
#include "File.h"
#include "savejpg.h"
#include <stdlib.h>
#include <stdio.h>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

/**
 *	Ham setFontSize - Thiet lap kich thuoc font chu
 *  @param  int   Kich thuoc font chu can thiet lap
 *	@return NULL
 */
void Menu::setFontSize(int size) {
	CONSOLE_FONT_INFOEX info = { 0 };

	info.cbSize = sizeof(info);
	info.dwFontSize.Y = size;
	info.FontWeight = FW_NORMAL;
	wcscpy_s(info.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}

/**
 *	Ham resizeConsole - Thiet lap lai kich thuoc man hinh
 *  @param  NULL
 *	@return NULL
 */
void Menu::resizeConsole() {
	HWND console = GetConsoleWindow();
	RECT rect;
	GetWindowRect(console, &rect);

	MoveWindow(console, rect.left, rect.top, this->width, this->height, TRUE);
}

/**
 *	Ham gotoXY - Di chuyen con chay tren console den toa do (x, y)
 *  @param  int	  Toa do truc nam ngang
 *  @param  int	  Toa do truc thang dung
 *	@return NULL
 */
void Menu::gotoXY(int x, int y) {
	COORD coord = { (SHORT)x, (SHORT)y };

	SetConsoleCursorPosition(consoleHandle, coord);
}

/**
 *	Ham putCharacterToConsole - Dua ki tu vao vi tri (x, y) tren man hinh
 *  @param  char  Ki tu can dua ra man hinh
 *  @param  int	  Toa do truc nam ngang
 *  @param  int	  Toa do truc thang dung
 *	@return NULL
 */
void Menu::putCharacterToConsole(char c, int x, int y)
{
	DWORD written;
	COORD coord = { (SHORT)x, (SHORT)y };

	FillConsoleOutputCharacter(consoleHandle, c, 1, coord, &written);
}

/**
 *	Ham putStringToConsole - Dua chuoi vao vi tri (x, y) tren man hinh
 *  @param  const char*  Chuoi can dua ra man hinh
 *  @param  int	         Toa do truc nam ngang
 *  @param  int	         Toa do truc thang dung
 *	@return NULL
 */
void Menu::putStringToConsole(const char str[], int x, int y) {
	DWORD written;
	COORD coord = { (SHORT)x, (SHORT)y };

	WriteConsoleOutputCharacter(consoleHandle, str, strlen(str), coord, &written);
}

/**
 *	Ham drawTitle - Ve tieu de cua tung muc menu ra man hinh, voi lua chon co khung hoac khong co khung (mac dinh)
 *  @param  const char*  Tieu de
 *  @param  int	         Toa do truc nam ngang
 *  @param  int	         Toa do truc thang dung
 *  @param  bool	     Co khong hay khong (mac dinh la khong)
 *	@return NULL
 */
void Menu::drawTitle(const char title[], int x, int y, bool haveFrame) {

	putStringToConsole(title, x, y);

	putCharacterToConsole(201, 0, 0);
	putCharacterToConsole(187, 53, 0);

	if (haveFrame) {
		putCharacterToConsole(200, 0, 34);
		putCharacterToConsole(188, 53, 34);

		for (int i = 1; i < 53; i++) {
			putCharacterToConsole(205, i, 0);
			putCharacterToConsole(205, i, 34);
		}

		for (int i = 1; i < 34; i++) {
			putCharacterToConsole(186, 0, i);
			putCharacterToConsole(186, 53, i);
		}

		for (int i = 1; i < 53; i++) {
			putCharacterToConsole(205, i, 4);
		}
	}
	else {
		putCharacterToConsole(200, 0, 4);
		putCharacterToConsole(188, 53, 4);

		for (int i = 1; i < 53; i++) {
			putCharacterToConsole(205, i, 0);
		}

		for (int i = 1; i < 4; i++) {
			putCharacterToConsole(186, 0, i);
			putCharacterToConsole(186, 53, i);
		}

		for (int i = 1; i < 53; i++) {
			putCharacterToConsole(205, i, 4);
		}
	}
}

/**
 *	Ham draw - Ve menu
 *  @param  NULL
 *	@return NULL
 */
void Menu::draw() {
	system("cls");

	//So ki tu toi da theo truc x la 0-53, truc y la 0-34
	while (1) {

		drawTitle("DO AN 2 - CAU TRUC DU LIEU & GIAI THUAT", 7, 2, true);

		for (int i = 10; i <= 44; i++) {
			putCharacterToConsole(196, i, 11);
			putCharacterToConsole(196, i, 15);
			putCharacterToConsole(196, i, 19);
			putCharacterToConsole(196, i, 23);
			putCharacterToConsole(196, i, 27);
		}

		putStringToConsole("1. LZSS compression", 14, 13);
		putStringToConsole("2. Huffman compression", 14, 17);
		putStringToConsole("3. JPEG compression", 14, 21);
		putStringToConsole("4. Exit", 14, 25);

		putCharacterToConsole(219, 20, 29);
		putStringToConsole("Option:", 22, 29);

		gotoXY(32, 29);

		scanf_s("%d", &option);

		if (1 <= option && option < 4) {
			break;
		}

		if (option == 4) {
			system("cls");
			exit(0);
		}
	}
}

/* Các con trỏ hàm tương ứng với các thuật toán nén, giải nén */

void encodeLZSS(FILE* inFile, FILE* outFile) {
	LZSS* lzss = new LZSS(1024);
	lzss->encodeLZSS(inFile, outFile);
	delete lzss;
}

void decodeLZSS(FILE* inFile, FILE* outFile) {
	LZSS* lzss = new LZSS(1024);
	lzss->decodeLZSS(inFile, outFile);
	delete lzss;
}

/* */

/**
 *	Ham run - Khoi chay menu
 *  @param  NULL
 *	@return NULL
 */
void Menu::run() {
	int inputChar;

	resizeConsole();
	setFontSize(FONT_SIZE);

	FileUtils* fileUtils = new FileUtils();


	while (1) {
		draw();

		switch (option) {
		case 1:
			char basePath[MAX_LENGTH_OF_PATH];
			char destCompressPath[MAX_LENGTH_OF_PATH];
			char destDecompressPath[MAX_LENGTH_OF_PATH];

			do {
				system("cls");
				gotoXY(0, 0);
				drawTitle("LZSS COMPRESSION", 17, 2);
				gotoXY(0, 6);

				printf("1. Compress\n");
				printf("2. Decompress\n");

				do {
					printf("\nOption : ");
					scanf_s("%d", &option);

					if (option > 2 || option < 0) {
						printf("Invalid option\n");
					}
				} while (option > 2 || option < 0);

				if (option == 1) {

					printf("Compressing's path: ");
					rewind(stdin);
					gets_s(basePath);

					if (fileUtils->checkPathOfFolder(basePath) || fileUtils->checkPathOfFile(basePath)) {
						fileUtils->drawDirectoryTree(basePath);

						printf("Compress into file (file path): ");
						rewind(stdin);
						gets_s(destCompressPath);

						if (!fileUtils->checkPathOfFolder(destCompressPath)) {
							printf("\nCompresing...\n");
							auto t1 = Clock::now();
							fileUtils->compress(encodeLZSS, basePath, destCompressPath);
							auto t2 = Clock::now();
							printf("Compressing time: %d (ms).\n\n", std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
						}
						else {
							printf("Invalid file path!\n");
						}
					}
					else {
						printf("Invalid path!\n");
					}
				}
				else {
					rewind(stdin);
					printf("\nDecompres Path: ");
					gets_s(basePath);

					if (fileUtils->checkPathOfFile(basePath)) {
						printf("Decompress into folder: ");
						rewind(stdin);
						gets_s(destDecompressPath);

						if (!fileUtils->checkPathOfFile(destDecompressPath)) {
							printf("\nDecompresing...\n");
							auto t1 = Clock::now();
							fileUtils->decompress(decodeLZSS, basePath, destDecompressPath);
							auto t2 = Clock::now();
							printf("Decompressing time: %d (ms).\n\n", std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
						}
						else {
							printf("Invalid folder path!\n");
						}
					}
					else {
						printf("Invalid file path!\n");
					}
				}

				printf("\nNhap Enter de tiep tuc, phim bat ki de quay ve menu!\n");
				inputChar = _getch();

			} while (inputChar == 13);

			break;

		case 2:
			do {
				system("cls");
				gotoXY(0, 0);
				drawTitle("HUFFMAN COMPRESSION", 17, 2);
				gotoXY(0, 6);

				printf("1. Compress\n");
				printf("2. Decompress\n");

				do {
					printf("\nOption : ");
					scanf_s("%d", &option);

					if (option > 2 || option < 0) {
						printf("Invalid option\n");
					}
				} while (option > 2 || option < 0);

				if (option == 1)
				{
					string inName;
					string outName;
					cout << "Link of file compress: ";
					cin >> inName;
					cout << "Link of file after compress: ";
					cin >> outName;
					printf("\nCompresing...\n");
					auto t1 = Clock::now();
					compressionData(inName, outName);
					auto t2 = Clock::now();
					printf("Compressing time: %d (ms).\n\n", chrono::duration_cast<chrono::milliseconds>(t2 - t1).count());
				}
				else {
					string inNamee;
					string outNamee;
					cout << "Link of file decode: ";
					cin >> inNamee;
					cout << "Link of file after decode: ";
					cin >> outNamee;
					printf("\nDecoding...\n");
					auto t1 = Clock::now();
					decodedFile(inNamee, outNamee);
					auto t2 = Clock::now();
					printf("Decoding time: %d (ms).\n\n", chrono::duration_cast<chrono::milliseconds>(t2 - t1).count());
				}
				printf("\nNhap Enter de tiep tuc, phim bat ki de quay ve menu!\n");
				inputChar = _getch();
			} while (inputChar == 13);
			break;
		case 3:
			do {
				system("cls");
				gotoXY(0, 0);
				drawTitle("JPEG COMPRESSION", 17, 2);
				gotoXY(0, 6);

				printf("Compressing's iamge path: ");
				rewind(stdin);
				gets_s(basePath);

				printf("Compress into image file (jpg): ");
				rewind(stdin);
				gets_s(destCompressPath);
				auto t1 = Clock::now();
				SaveJpgFile(basePath, destCompressPath);
				auto t2 = Clock::now();
				printf("\nCompressing time: %d (ms).\n\n", chrono::duration_cast<chrono::milliseconds>(t2 - t1).count());

				printf("\nNhap Enter de tiep tuc, phim bat ki de quay ve menu!\n");
				inputChar = _getch();
			} while (inputChar == 13);

			break;
		}
	}

	delete fileUtils;
}

/**
 *	Constructor - Thiet lap kich thuoc man hinh
 *  @param  NULL
 *	@return NULL
 */
Menu::Menu() {
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	this->width = WIDTH;
	this->height = HEIGHT;
}
