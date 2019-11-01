#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#include<stdio.h>
#include<string.h>
#include<errno.h>

/*自作bool*/
#include"myBool.h"

/*自作構造体*/
#include"pixelDataRGB.h"
#include "bitmapInfoHeader.h"

/*自作関数*/
#include"strJoin.h"
#include"calc.h"
#include"isBitmapFile.h"
#include"getBitmapInfoHeader.h"
#include "getPictureData.h"
#include "writeColorPaletteGrayScale.h"
#include "writeBitmapFileHeader.h"
#include "writeBitmapInfoHeader.h"

/*24bit形式から8bit形式に変換したファイルの名前の前に付ける文字列*/
#define ADD_FILE_NAME "convert_"

int main(void) {
	FILE* inputFile = NULL;
	FILE* outputFile = NULL;

	/*変換するファイルの名前*/
	char inputFileName[] = "24sample2.bmp";

	/*変換後のファイル名*/
	char* outputFileName = strJoin(ADD_FILE_NAME, inputFileName);

	inputFile = fopen(inputFileName, "rb");
	if (NULL == inputFile) {
		/*変換前のファイルを開くのに失敗した場合の処理*/
		printf("Failed to open %s\n", inputFileName);
		exit(1);
	}

	outputFile = fopen(outputFileName, "wb");


	/*ファイルヘッダのファイルタイプがBMかどうかの判定*/
	if (!isBitmapFile(inputFile)) {
		printf("This file is not BMP file\n");
		exit(1);
	}

	/*情報ヘッダの情報を取得し構造体へ*/
	bitmapInfoHeader* pBIH = getBitmapInfoHeader(inputFile);

	/*画像データの情報を取得*/
	unsigned char* pictureData = getPictureData(inputFile, pBIH);

	fclose(inputFile);


	/*ファイルヘッダの書き込み*/
	writeBitmapFileHeader(outputFile, pBIH);

	/*情報ヘッダの書き込み*/
	writeBitmapInfoHeader(outputFile, pBIH);

	/*カラーパレッドの書き込み*/
	writeColorPaletteGrayScale(outputFile);


	unsigned long  width = pBIH->biWidth;
	unsigned long  height = abs(pBIH->biHeight);

	free(pBIH);

	/*インプットファイルの画像データのパディング数*/
	unsigned char inputPaddingNum = calcMultipleOf4(width * 3) - width * 3;

	/*アウトプットファイルの画像データのパディング数*/
	unsigned char outputPaddingNum = calcMultipleOf4(width) - width;

	/*変換前画像の画像データ格納用構造体*/
	pixelDataRGB inputFilePixelData = {.red = 0,.green = 0,.blue = 0};

	/*インプットファイルの画像データの、一行あたりのバイト数*/
	unsigned long bytesNumInRowOfInputFile = width * 3 + inputPaddingNum;

	for (unsigned long h = 0; h < height; h++) {

		/*プログレス表示*/
		printf("\r[%lu/%lu](%lu%%)", h + 1, height, (h + 1) * 100 / height);

		for (unsigned long w = 0; w < width; w++) {
			inputFilePixelData.blue = pictureData[w * 3 + bytesNumInRowOfInputFile * h];
			inputFilePixelData.green = pictureData[w * 3 + 1 + bytesNumInRowOfInputFile * h];
			inputFilePixelData.red = pictureData[w * 3 + 2 + bytesNumInRowOfInputFile * h];

			unsigned char luminance = calcLuminance(&inputFilePixelData);
			fwrite(&luminance, sizeof(luminance), 1, outputFile);
		}

		/*パディングの書き込み*/
		for (unsigned char padd = 0; padd < outputPaddingNum; padd++) {
			fputc(0x00, outputFile);
		}
	}

	free(pictureData);
	fclose(outputFile);

	printf("\n\nConversion succeeded.\n\nConverted file name is %s\n", outputFileName);

	/*関数strJoin内でmallocを使用しているので、メモリ開放*/
	free(outputFileName);

	/*メモリリーク検出*/
	_CrtDumpMemoryLeaks();

	return 0;
}
