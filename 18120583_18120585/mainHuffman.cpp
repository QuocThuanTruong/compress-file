#include "file.h"
#include "FileUtils.h"
#include "LZSS.h"
#include <time.h>
#include <chrono>
int main() {
	string in = "D:\\aaa.txt";
	string out = "D:\\kq.txt";
	string c = "D:\\aaaaa.txt";
	auto t1 = Clock::now();
	compressionData(in, out);
	auto t2 = Clock::now();
	printf("Decompressing time: %d (ms).", std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
	decodedFile(out, c);
	system("pause");
	return 0;
}