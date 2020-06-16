#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#pragma once
#include<iostream>
#define MAX_CHAR 256
//#define MAX_CHAR
#define MAX_CODE 256
#include<fstream>
#include<string>
#include<vector>
using namespace std;

struct Node // nốt trên cây
{
	char c; // kí tự
	long frequence; // tần số
	Node* pLeft, * pRight; // nốt trái, phải
};

struct Tree // cây huffman
{
	unsigned size; // kích cỡ
	Node** array; // mảng node
};

struct Char // kí tự
{
	char code[MAX_CODE]; // mã code ( dạng nhị phân )
	unsigned size;  // kích thước
	char c; // kí tự ( dạng thập phân )
	long frequence; // tần suất
};

//khởi tạo một node
Node* creatNode(char c, long freq);

//hiệu chỉnh cây 
void heapify(Tree* tree, int i);

//hoán đổi 2 node
void swapNode(Node*& a, Node*& b);

//kiểm tra cây có đúng còn 1 node hay không
bool isSizeOne(Tree* tree);

//lấy node có giá trị nhỏ nhất trong cây
Node* getMinNode(Tree* tree);

//chèn một node vào cây huffman
void insertNodeIntoTree(Tree* tree, Node* Node);

//khởi tạo cây minheap
Tree* creatMinHeap(unsigned size);

//dựng cây minheap
void buildMinHeap(Tree* tree);
Tree* creatAndBuildMinHeap(char arr[], long frequence[], unsigned size);

// tạo và dựng cây minheap
Tree* buildHuffmanTree(char arr[], long frequence[], unsigned size);

//xây dựng cây huffman
vector<Char> findCharTable(char arr[], long frequence[], unsigned size);

// chuyển từ cơ số 10 sang 2
string convertDecimalToBinary(int Deci);

// tìm kí tự trên cây
void findChar(Node* tree, vector<Char>& table, char code[], int top);
#endif // !_HUFFMAN_H_#pragma once