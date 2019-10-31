#include<stdio.h>
#include <stdlib.h>

#include "bitmapInfoHeader.h"

unsigned char* getPictureData(FILE* file,bitmapInfoHeader* pBIH) {

	/*変換前の画像データのサイズを計算*/
	unsigned long pictureDataSize = calcMultipleOf4(pBIH->biWidth * 3) * pBIH->biHeight;

	/*画像データの情報を格納する領域の確保*/
	unsigned char *pictureData = (unsigned char *)malloc(pictureDataSize);
	memset(pictureData, 0x00, pictureDataSize);

	if (NULL == pictureData) {
		/*メモリの割当に失敗した際の処理*/
		printf("Faild to allocate memory\n");
		exit(1);
	}

	/*画像データの情報を取得*/
	fread(pictureData, pictureDataSize, 1, file);

	return pictureData;
}