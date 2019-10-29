#pragma once
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