#include<stdio.h>
#include <stdlib.h>

#include "bitmapInfoHeader.h"
#include"pixelDataRGB.h"

pixelDataRGB* getPictureData(FILE* pFile,bitmapInfoHeader* pBIH) {

	/*ピクセル数を計算*/
	unsigned long pixelNum = pBIH->biWidth * abs(pBIH->biHeight);

	/*画像データの情報を格納する領域の確保*/
	pixelDataRGB* pictureData = (pixelDataRGB*)malloc(sizeof(pixelDataRGB) * pixelNum);
	memset(pictureData, 0x00, pixelNum * sizeof(*pictureData));

	if (NULL == pictureData) {
		/*メモリの割当に失敗した際の処理*/
		printf("Faild to allocate memory\n");
		exit(1);
	}

	/*画像データの情報を取得*/
	//fread(pictureData, pixelNum * sizeof(*pictureData), 1, file);

	/*一行ごとのパディングの数*/
	unsigned char paddingNum = calcMultipleOf4(pBIH->biWidth * 3) - pBIH->biWidth * 3;

	unsigned long count = 0;
	for (unsigned long h = 0; h < pBIH->biHeight; h++) {
		for (unsigned long w = 0; w < pBIH->biWidth; w++) {
			fread(pictureData + count, sizeof(pixelDataRGB), 1, pFile);
			count++;
		}
		/*ファイルの位置指定子をパディングの分だけずらす*/
		fseek(pFile, paddingNum, SEEK_CUR);
	}

	return pictureData;
}