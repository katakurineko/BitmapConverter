#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmapInfoHeader.h"

bitmapInfoHeader* getBitmapInfoHeader(FILE* pFile) {

	bitmapInfoHeader* pBIH = (bitmapInfoHeader*)malloc(sizeof(bitmapInfoHeader));

	if (NULL == pBIH) {
		/*メモリの割当に失敗した際の処理*/
		printf("Faild to allocate memory\n");
		exit(1);
	}

	/*返り値として返したい構造体bitmapInfoHeaderを初期化*/
	pBIH->biSize = 0;
	pBIH->biWidth = 0;
	pBIH->biHeight = 0;
	pBIH->biPlanes = 1;
	pBIH->biBitCount = 0;
	pBIH->biCompression = 0;
	pBIH->biSizeImage = 0;
	pBIH->biXPelsPerMeter = 0;
	pBIH->biYPelsPerMeter = 0;
	pBIH->biClrUsed = 0;
	pBIH->biClrImportant = 0;

	/*情報ヘッダのサイズを取得*/
	unsigned char bitmapInfoHeaderSize = fgetc(pFile);

	/*windowsBitmapは40バイトだが、OS/2の場合は12バイトらしいので、
	windowsBitmapかどうかを判定*/
	if (WINDOWS_BITMAP_FILE_SIZE != bitmapInfoHeaderSize) {
		printf("This bitmap file is not windowsBitmap\n");
		exit(1);
	}

	/*ファイルの位置指定子を1バイト戻して、情報ヘッダの開始位置へ*/
	fseek(pFile, -1, SEEK_CUR);

	/*情報ヘッダの情報を格納する領域の確保*/
	char* bitmapInfoHeader = (char *)malloc(bitmapInfoHeaderSize);
	if (NULL == bitmapInfoHeader) {
		/*メモリの割当に失敗した際の処理*/
		printf("Faild to allocate memory\n");
		exit(1);
	}
	memset(bitmapInfoHeader, 0x00, bitmapInfoHeaderSize);

	/*情報ヘッダの情報を取得*/
	fread(bitmapInfoHeader, bitmapInfoHeaderSize, 1, pFile);

	/*画像の幅、高さを取得*/
	memcpy(&pBIH->biWidth, &bitmapInfoHeader[4], sizeof(pBIH->biWidth));
	memcpy(&pBIH->biHeight, &bitmapInfoHeader[8], sizeof(pBIH->biHeight));

	/*1画素あたりのデータサイズを取得*/
	memcpy(&pBIH->biBitCount, &bitmapInfoHeader[14], sizeof(pBIH->biBitCount));

	/*24bit形式かどうかの判定*/
	if (24 != pBIH->biBitCount) {
		printf("This file is not 24bits");
		exit(1);
	}

	/*圧縮形式を取得*/
	memcpy(&pBIH->biCompression, &bitmapInfoHeader[16], sizeof(pBIH->biCompression));

	/*ファイルが圧縮されていないかの判定*/
	if (NOT_COMPRESSION != pBIH->biCompression) {
		printf("File is compressed");
		exit(1);
	}


	free(bitmapInfoHeader);

	return pBIH;
}