#pragma once

#define WINDOWS_BITMAP_FILE_SIZE 40

#define NOT_COMPRESSION 0

#define BI_PLANES_VALUE 1

/*1画素あたりのデータサイズ (bit)*/
#define BI_BIT_COUNT_VALUE 8

/*圧縮形式(無圧縮)*/
#define NOT_COMPRESSION 0

/*縦,横方向それぞれの解像度*/
#define BI_X_PELS_PER_METER_VALUE 0
#define BI_Y_PELS_PER_METER_VALUE 0

/*8bit形式の場合のパレット数*/
#define BI_CLR_USED_8BIT 256

/*重要なパレッドのインデックス(0はすべて重要の意味)*/
#define BI_CLR_IMPORTANT_VALUE 0


typedef struct bitmapInfoHeader {
	unsigned long biSize;
	unsigned long  biWidth;
	long  biHeight;
	unsigned short  biPlanes;
	unsigned short  biBitCount;
	unsigned long biCompression;
	unsigned long biSizeImage;
	unsigned long  biXPelsPerMeter;
	unsigned long  biYPelsPerMeter;
	unsigned long biClrUsed;
	unsigned long biClrImportant;
}bitmapInfoHeader;