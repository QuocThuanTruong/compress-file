#include "Huffman.h"

/**
 *	Hàm creatNode - khởi tạo một nốt .
 *	@param		char: kí tự  long: tần số của kí tự đó 	.
 *	@return		Node             : nốt được tạo
 */
Node* creatNode(char c, long freq)
{
	Node* node = new Node;
	node->c = c;
	node->frequence = freq;
	node->pLeft = NULL;
	node->pRight = NULL;

	return node;

}
/**
 *	Hàm creatMinHeap - khởi tạo cây minHeap .
 *	@param		unsigned: kích thước của cây 	.
 *	@return		Tree            : cây được tạo
 */
Tree* creatMinHeap(unsigned size)
{
	Tree* tree = new Tree;
	tree->size = size;
	tree->array = new Node * [size];
	for (int i = 0; i < size; i++)
		tree->array[i] = new Node;

	return tree;
}

/**
 *	Hàm creatAndBuildMinHeap - tạo và xây dựng cây min heap .
 *	@param		char: mảng kí tự, long: mảng tần số xuất hiện của mỗi kí tự, unsigned: kích thước của cây
 *	@return		Tree             : cây được tạo
 */
Tree* creatAndBuildMinHeap(char arr[], long frequence[], unsigned size)
{

	Tree* tree = creatMinHeap(size);

	for (int i = 0; i < size; i++)
	{
		tree->array[i] = creatNode(arr[i], frequence[i]);
	}

	buildMinHeap(tree);
	return tree;
}
/**
 *	Hàm swapnode
 */
void swapNode(Node*& a, Node*& b)
{
	Node* tmp = a;
	a = b;
	b = tmp;
}

/**
 *	Hàm heapify - hàm hiệu chỉnh cây tại 1 vị trí .
 *	@param		Tree: cây   int : vị trí cần hiệu chỉnh 	.
 *	@return	    NULL
 */
void heapify(Tree* tree, int i)
{
	int l = 2 * i + 1;
	int r = 2 * i + 2;

	long minIndex = i;

	if (l < tree->size && tree->array[l]->frequence < tree->array[minIndex]->frequence)
		minIndex = l;
	if (r < tree->size && tree->array[r]->frequence < tree->array[minIndex]->frequence)
		minIndex = r;

	if (minIndex != i)
	{
		swapNode(tree->array[minIndex], tree->array[i]);
		heapify(tree, minIndex);
	}

}
/**
 *	Hàm buildMinHeap - hàm xây dựng minheap ( đã hiệu chỉnh ) .
 *	@param		Tree: cây chưa hiệu chỉnh
 *	@return		NULL
 */
void buildMinHeap(Tree* tree)
{
	int n = tree->size - 1;
	for (int i = (n - 1) / 2; i >= 0; i--)
	{
		heapify(tree, i);
	}
}
/**
 *	Hàm isSizeOne - Kiểm tra size cây có bằng 1 không.
 *	@param		Tree: cây 	.
 *	@return		True nếu size = 1, false nếu size = 0
 */
bool isSizeOne(Tree* tree)
{
	return tree->size == 1;
}
/**
 *	Hàm getMinNode -lấy node có giá trị nhỏ nhất .
 *	@param		Tree: cây cần lấy 	.
 *	@return		Node
 */
Node* getMinNode(Tree* tree)
{
	Node* tmp = tree->array[0];

	tree->array[0] = tree->array[tree->size - 1];
	--tree->size;
	heapify(tree, 0);
	return tmp;
}
/**
 *	Hàm insertNodeIntoTree - chèn một nốt vào cây .
 *	@param		Tree: cây Node: node cần chèn  	.
 *	@return		NULL
 */
void insertNodeIntoTree(Tree* tree, Node* Node)
{
	++tree->size;
	int i = tree->size - 1;

	while (i && Node->frequence < tree->array[(i - 1) / 2]->frequence)
	{
		tree->array[i] = tree->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	tree->array[i] = Node;

}

/**
 *	Hàm buildHuffmanTree - xây dựng cây huffman .
 *	@param		char: mảng kí tự, long: mảng tần suất, unsigned: kích thước.
 *	@return		Tree: cây được dựng theo kiểu huffman
 */
Tree* buildHuffmanTree(char arr[], long frequence[], unsigned size)
{
	Tree* tree = creatAndBuildMinHeap(arr, frequence, size);
	Node* pLeft, * pRight, * top;

	while (isSizeOne(tree) != 1)
	{
		pLeft = getMinNode(tree);
		pRight = getMinNode(tree);
		top = creatNode('$', pLeft->frequence + pRight->frequence);
		top->pLeft = pLeft;
		top->pRight = pRight;
		insertNodeIntoTree(tree, top);
	}
	return tree;

}

/**
 *	Hàm findCharTable - tìm bảng kí tự .
 *	@param		char: mảng kí tự, long: mảng tần suất, unsigned: kích thước.
 *	@return		vector<Char>: vector kiểu kí tự ( mã code, tần suất, kí tự, kích thước )
 */
vector<Char> findCharTable(char arr[], long frequence[], unsigned size)
{
	Tree* tree = buildHuffmanTree(arr, frequence, size);
	char code[MAX_CODE];
	vector<Char>arrCode;
	findChar(tree->array[0], arrCode, code, 0);
	return arrCode;
}

/**
 *	Hàm findChar - tìm kí tự .
 *	@param		Node: nốt chứa kí tự, vector<Char> vector chứa ds kí tự, int: vị trí bắt đầu .
 *	@return		NULL
 */
void findChar(Node* tree, vector<Char>& table, char code[], int top)
{
	if (tree->pLeft)
	{
		code[top] = '0';
		findChar(tree->pLeft, table, code, top + 1);
	}
	if (tree->pRight)
	{
		code[top] = '1';
		findChar(tree->pRight, table, code, top + 1);
	}
	if (tree->pLeft == NULL && tree->pRight == NULL)
	{
		Char x;
		x.c = tree->c;
		x.size = top;
		x.frequence = tree->frequence;
		for (int i = 0; i < top; i++)
			x.code[i] = code[i];
		table.push_back(x);
	}
}


// hàm cover từ cơ số 10 sang 2
string convertDecimalToBinary(int Deci)
{
	string kq = "";
	if (Deci < 0)
		Deci += 256;
	int i = 8;
	for (int j = 7; j >= 0; j--)
	{
		kq += (((Deci >> j) & 1) + 48);
	}
	--i;

	return kq;
}
