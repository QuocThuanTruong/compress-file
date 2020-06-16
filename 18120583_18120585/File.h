#ifndef _FILE_H_
#define _FILE_H_

#pragma once
#include "huffman.h"

//đọc file lấy dữ liệu
bool readFile(string name, long data[]);

//đọc file thống kê tần số mỗi kí tự
bool DataAndFReq(string name, char data[], unsigned& size, long freq[]);

// nén dữ liệu vào file khác
void compressionData(string inName, string outName);

//tạo chuỗi nén dữ liệu
string encodedHuffmanData(string name, vector<Char>codeTable);

//tạo chuỗi dữ liệu sau giải nén
void decodedData(ifstream& inFile, string outName);

//giải nén
void decodedFile(string inName, string outName);
#endif // !_FILE_H_