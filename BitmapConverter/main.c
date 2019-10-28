#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#include<stdio.h>
#include<string.h>
#include<errno.h>

#include"strJoin.h"
#include"calc.h"
#include"pixelDataRGB.h"


/*24bit形式から8bit形式に変換したファイルの名前の前に付ける文字列*/
#define ADD_FILE_NAME "convert_"

#define BITMAP_FILEHEADER_REGION_SIZE 14
#define BF_TYPE_REGION_SIZE 2
/*8bit形式のオフセットのサイズ
(ファイルヘッダ)14 + (情報ヘッダ)40 + (カラーパレッド)256*4 */
#define BF_OFF_BITS_VALUE 1078

#define WINDOWS_BITMAP_FILE_SIZE 40

#define BI_PLANES_VALUE 1

#define BI_BIT_COUNT_VALUE 8

#define NOT_COMPRESSION 0

#define BI_X_PELS_PER_METER_VALUE 0
#define BI_Y_PELS_PER_METER_VALUE 0

/*8bit形式の場合のパレット数*/
#define BI_CLR_USED_8BIT 256

/*重要なパレッドのインデックス(0はすべて重要の意味)*/
#define BI_CLR_IMPORTANT_VALUE 0

/*カラーパレッドの予約領域の値*/
#define RGB_RESERVED 0

int main(void) {
	FILE *inputFile = NULL;
	FILE *outputFile = NULL;

	/*変換するファイルの名前*/
	char inputFileName[] = "24sample2.bmp";

	/*変換後のファイル名*/
	char *outputFileName = strJoin(ADD_FILE_NAME, inputFileName);

	inputFile = fopen(inputFileName, "rb");
	outputFile = fopen(outputFileName, "wb");

	/*ファイルヘッダの情報を格納する領域を確保*/
	char bitmapFileHeader[BITMAP_FILEHEADER_REGION_SIZE];

	/*ファイルヘッダの情報を取得*/
	fread(&bitmapFileHeader, sizeof(char), BITMAP_FILEHEADER_REGION_SIZE, inputFile);

	/*ファイル形式.
	2バイト（2文字）+ヌル文字分の領域を確保する.*/
	char bfType[BF_TYPE_REGION_SIZE + sizeof(char)];

	/*ファイル形式を取得する*/
	snprintf(bfType, BF_TYPE_REGION_SIZE + sizeof(char), "%s", bitmapFileHeader);

	if (0 != strcmp("BM", bfType)) {
		/*ファイルヘッダのファイルタイプがBMでなかった際の処理*/
		printf("This file is not BMP file\n");
		exit(1);
	}



	/*情報ヘッダのサイズを取得*/
	unsigned char bitmapInfoHeaderSize = fgetc(inputFile);
	if (WINDOWS_BITMAP_FILE_SIZE != bitmapInfoHeaderSize) {
		/*windowsBitmapは40バイト固定だが、OS/2の場合は12バイトらしいので、OS/2だった場合の処理*/
		printf("This bitmap file is not windowsBitmap\n");
		exit(1);
	}

	/*ファイルの位置指定子を1バイト戻して、情報ヘッダの開始位置へ*/
	fseek(inputFile, -1, SEEK_CUR);

	/*情報ヘッダの情報を格納する領域*/
	char *bitmapInfoHeader = (char *)malloc(bitmapInfoHeaderSize);
	if (NULL == bitmapInfoHeader) {
		/*メモリの割当に失敗した際の処理*/
		printf("Faild to allocate memory\n");
		exit(1);
	}

	/*情報ヘッダの情報を取得*/
	fread(bitmapInfoHeader, sizeof(char), bitmapInfoHeaderSize, inputFile);

	/*画像の幅、高さを取得*/
	unsigned long width;
	memcpy(&width, &bitmapInfoHeader[4], sizeof(width));
	unsigned long height;
	memcpy(&height, &bitmapInfoHeader[8], sizeof(height));

	/*1画素あたりのデータサイズ*/
	short bitsPerPixel;
	memcpy(&bitsPerPixel, &bitmapInfoHeader[14], sizeof(bitsPerPixel));

	if (bitsPerPixel != 24) {
		/*ファイルが24bitでなかった際の処理*/
		printf("This file is not 24bits");
		exit(1);
	}

	/*圧縮形式*/
	int compression = bitmapInfoHeader[16];
	memcpy(&compression, &bitmapInfoHeader[16], sizeof(compression));

	if (NOT_COMPRESSION != compression) {
		/*ファイルが圧縮されている際の処理*/
		printf("File is compressed");
		exit(1);
	}

	free(bitmapInfoHeader);

	/*データ上の画像の幅*/
	unsigned long widthMultipleOf4 = calcMultipleOf4(width);

	/*ピクセル数*/
	unsigned long outputFileAllPixelNum = widthMultipleOf4 * height;

	/*インプットファイルの画像データのパディング数*/
	unsigned char inputPaddingNum = calcMultipleOf4(width * 3) - width * 3;

	/*アウトプットファイルの画像データのパディング数*/
	unsigned char outputPaddingNum = calcMultipleOf4(width) - width;

	/*画像データのサイズ*/
	unsigned long pictureDataSize = outputFileAllPixelNum * 3;

	/*ファイル形式を書き込み*/
	fwrite(bfType, strlen(bfType), 1, outputFile);

	/*ファイルサイズを計算して書き込み*/
	unsigned long bfSize = BF_OFF_BITS_VALUE + outputFileAllPixelNum;
	fwrite(&bfSize, sizeof(bfSize), 1, outputFile);

	/*予約領域を書き込み*/
	signed short bfReserved1 = 0;
	fwrite(&bfReserved1, sizeof(bfReserved1), 1, outputFile);
	signed short bfReserved2 = 0;
	fwrite(&bfReserved2, sizeof(bfReserved2), 1, outputFile);

	/*ファイル先頭から画像データまでのオフセットを書き込み*/
	unsigned long bfOffBits = BF_OFF_BITS_VALUE;
	fwrite(&bfOffBits, sizeof(bfOffBits), 1, outputFile);

	/*情報ヘッダのサイズを書き込み*/
	unsigned long biSize = WINDOWS_BITMAP_FILE_SIZE;
	fwrite(&biSize, sizeof(biSize), 1, outputFile);

	/*画像の幅と高さを書き込み*/
	fwrite(&width, sizeof(width), 1, outputFile);
	fwrite(&height, sizeof(height), 1, outputFile);

	/*プレーン数を書き込み*/
	unsigned short biPlanes = BI_PLANES_VALUE;
	fwrite(&biPlanes, sizeof(biPlanes), 1, outputFile);

	/*1画素あたりのデータサイズを書き込み*/
	unsigned short biBitCount = BI_BIT_COUNT_VALUE;
	fwrite(&biBitCount, sizeof(biBitCount), 1, outputFile);

	/*圧縮形式を書き込み*/
	unsigned long biCompression = NOT_COMPRESSION;
	fwrite(&biCompression, sizeof(biCompression), 1, outputFile);

	/*画像データ部のサイズを書き込み*/
	unsigned long biSizeImage = outputFileAllPixelNum;
	fwrite(&biSizeImage, sizeof(biSizeImage), 1, outputFile);

	/*解像度を書き込み*/
	unsigned long biXPixPerMeter = BI_X_PELS_PER_METER_VALUE;
	unsigned long biYPixPerMeter = BI_Y_PELS_PER_METER_VALUE;
	fwrite(&biXPixPerMeter, sizeof(biXPixPerMeter), 1, outputFile);
	fwrite(&biYPixPerMeter, sizeof(biYPixPerMeter), 1, outputFile);

	/*パレット数の書き込み*/
	unsigned long biClrUsed = BI_CLR_USED_8BIT;
	fwrite(&biClrUsed, sizeof(biClrUsed), 1, outputFile);

	/*重要なパレットのインデックスを書き込み*/
	unsigned long biClrImportant = BI_CLR_IMPORTANT_VALUE;
	fwrite(&biClrImportant, sizeof(biClrImportant), 1, outputFile);

	/*カラーパレッドの作成*/
	unsigned char rgbColorPalette = 0;
	unsigned char colorPaletteReserved = 0;
	for (int i = 0; i < BI_CLR_USED_8BIT; i++) {
		for (int j = 0; j < 3; j++) {
			fwrite(&rgbColorPalette, sizeof(rgbColorPalette), 1, outputFile);
		}
		fwrite(&colorPaletteReserved, sizeof(colorPaletteReserved), 1, outputFile);
		rgbColorPalette++;
	}


	/*画像データの情報を格納する領域*/
	unsigned char *pictureData = (unsigned char *)malloc(pictureDataSize);

	if (NULL == pictureData) {
		/*メモリの割当に失敗した際の処理*/
		printf("Faild to allocate memory\n");
		exit(1);
	}

	/*画像データの情報を取得*/
	fread(pictureData, sizeof(char), pictureDataSize, inputFile);

	fclose(inputFile);

	/*変換前画像の画像データ格納用構造体*/
	pixelDataRGB inputFilePixelData;
	inputFilePixelData.red = 0x00;
	inputFilePixelData.green = 0x00;
	inputFilePixelData.blue = 0x00;

	/*インプットファイルの画像データの、一行あたりのバイト数*/
	unsigned long bytesNumInRowOfInputFile = width * 3 + inputPaddingNum;

	for (unsigned long h = 0; h < height; h++) {

		/*プログレス表示*/
		printf("\r[%lu/%lu](%lu%%)", h + 1, height, (h + 1) * 100 / height);

		for (unsigned long w = 0; w < width; w++) {
			inputFilePixelData.blue = pictureData[w * 3 + bytesNumInRowOfInputFile * h];
			inputFilePixelData.green = pictureData[w * 3 + 1 + bytesNumInRowOfInputFile * h];
			inputFilePixelData.red = pictureData[w * 3 + 2 + bytesNumInRowOfInputFile * h];
			unsigned char convertedData = calcLuminance(&inputFilePixelData);
			fwrite(&convertedData, sizeof(convertedData), 1, outputFile);
		}
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
