#include<stdio.h>

/*自作構造体*/
#include"pixelDataRGB.h"
#include "bitmapInfoHeader.h"

#include"isBitmapFile.h"
#include"getBitmapInfoHeader.h"
#include "getPictureData.h"
#include "writeColorPaletteGrayScale.h"
#include "writeBitmapFileHeader.h"
#include "writeBitmapInfoHeader.h"
#include "writePictureData.h"


void convertBitmap24bitTo8bit(FILE* pInputFile, FILE* pOutputFile) {

	/*ファイルヘッダのファイルタイプがBMかどうかの判定*/
	if (!isBitmapFile(pInputFile)) {
		printf("This file is not BMP file\n");
		exit(1);
	}

	/*情報ヘッダの情報を取得し構造体へ*/
	bitmapInfoHeader* pBIH = getBitmapInfoHeader(pInputFile);

	/*画像データの情報を取得*/
	pixelDataRGB* pPictureData = getPictureData(pInputFile, pBIH);


	/*ファイルヘッダの書き込み*/
	writeBitmapFileHeader(pOutputFile, pBIH);

	/*情報ヘッダの書き込み*/
	writeBitmapInfoHeader(pOutputFile, pBIH);

	/*カラーパレッドの書き込み*/
	writeColorPaletteGrayScale(pOutputFile);

	/*画像データの変換と書き込み*/
	writePictureData(pOutputFile, pBIH, pPictureData);


	free(pBIH);
	free(pPictureData);
}