#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmapInfoHeader.h"

bitmapInfoHeader* getBitmapInfoHeader(FILE* file) {

	/*返り値として返したい構造体bitmapInfoHeaderを初期化*/
	bitmapInfoHeader BIH = {.biSize = 0, .biWidth =0,.biHeight =0,
		.biPlanes =1,.biBitCount =0,.biCompression =0,.biSizeImage = 0,
		.biXPelsPerMeter =0,.biYPelsPerMeter =0,.biClrUsed =0,.biClrImportant =0};

	/*情報ヘッダのサイズを取得*/
	unsigned char bitmapInfoHeaderSize = fgetc(file);

	/*windowsBitmapは40バイトだが、OS/2の場合は12バイトらしいので、
	windowsBitmapかどうかを判定*/
	if (WINDOWS_BITMAP_FILE_SIZE != bitmapInfoHeaderSize) {
		printf("This bitmap file is not windowsBitmap\n");
		exit(1);
	}

	/*ファイルの位置指定子を1バイト戻して、情報ヘッダの開始位置へ*/
	fseek(file, -1, SEEK_CUR);

	/*情報ヘッダの情報を格納する領域の確保*/
	char* bitmapInfoHeader = (char *)malloc(bitmapInfoHeaderSize);
	memset(bitmapInfoHeader, 0x00, bitmapInfoHeaderSize);
	if (NULL == bitmapInfoHeader) {
		/*メモリの割当に失敗した際の処理*/
		printf("Faild to allocate memory\n");
		exit(1);
	}

	/*情報ヘッダの情報を取得*/
	fread(bitmapInfoHeader, bitmapInfoHeaderSize, 1, file);

	/*画像の幅、高さを取得*/
	memcpy(&BIH.biWidth, &bitmapInfoHeader[4], sizeof(BIH.biWidth));
	memcpy(&BIH.biHeight, &bitmapInfoHeader[8], sizeof(BIH.biHeight));

	/*1画素あたりのデータサイズを取得*/
	memcpy(&BIH.biBitCount, &bitmapInfoHeader[14], sizeof(BIH.biBitCount));

	/*24bit形式かどうかの判定*/
	if (24 != BIH.biBitCount) {
		printf("This file is not 24bits");
		exit(1);
	}

	/*圧縮形式を取得*/
	memcpy(&BIH.biCompression, &bitmapInfoHeader[16], sizeof(BIH.biCompression));

	/*ファイルが圧縮されていないかの判定*/
	if (NOT_COMPRESSION != BIH.biCompression) {
		printf("File is compressed");
		exit(1);
	}


	free(bitmapInfoHeader);

	return &BIH;
}