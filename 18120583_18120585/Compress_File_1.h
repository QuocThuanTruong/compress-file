#include <stdio.h>

struct NODE {
	unsigned char	c;		// ky tu
	int		freq;	// so lan xuat hien
	bool	used;	// danh dau node da xu ly chua 
	int		nLeft;	// chi so nut con nam ben trai
	int		nRight;	// chi so nut con nam ben phai
};

struct MABIT {
	char*	bits;
	int		soBit;
};

const int MAX_NODE = 256 * 9;
const int MAX_BIT_LEN = 10000;
NODE	huffTree[MAX_NODE];
MABIT	bangMaBit[256];

void KhoiTao();
void ThongKeTanSoXuatHien(char* tenFile);
void XuatBangThongKe();
bool Tim2PhanTuMin(int &i, int &j, int nNode);
int TaoCayHuffman();
void XuatCayHuffman(int node, int tab);
void DuyetCayHuffman(int node, char maBit[], int nMaBit);
void PhatSinhMaBit(int nRoot);
void XuatByte(unsigned char out, int soBitCoNghia);
void MaHoa1KyTu(unsigned char c, unsigned char &out, unsigned char &viTriBit);
void NenHuffman(char* inputFile);

