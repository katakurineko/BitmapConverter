#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#include<stdio.h>

/*自作関数*/
#include"strJoin.h"
#include"convertBitmap24bitTo8bit.h"

/*24bit形式から8bit形式に変換したファイルの名前の前に付ける文字列*/
#define ADD_FILE_NAME "convert_"

/*変換するファイルの名前*/
#define INPUT_FILE_NAME "24sample2.bmp"

int main(void) {
	FILE* pInputFile = NULL;
	FILE* pOutputFile = NULL;

	/*変換するファイルの名前*/
	char inputFileName[] = INPUT_FILE_NAME;

	/*変換後のファイル名*/
	char* outputFileName = strJoin(ADD_FILE_NAME, inputFileName);

	pInputFile = fopen(inputFileName, "rb");
	if (NULL == pInputFile) {
		/*変換前のファイルを開くのに失敗した場合の処理*/
		printf("Failed to open %s\n", inputFileName);
		exit(1);
	}

	pOutputFile = fopen(outputFileName, "wb");
	if (NULL == pOutputFile) {
		/*変換後のファイルを開くのに失敗した場合の処理*/
		printf("Failed to open %s\n", outputFileName);
		exit(1);
	}

	/*24bitのBitmapファイルを8bitに変更*/
	convertBitmap24bitTo8bit(pInputFile, pOutputFile);

	fclose(pInputFile);
	fclose(pOutputFile);

	printf("\n\nConversion succeeded.\n\nConverted file name is %s\n", outputFileName);

	/*関数strJoin内でmallocを使用しているので、メモリ開放*/
	free(outputFileName);

	/*メモリリーク検出*/
	_CrtDumpMemoryLeaks();

	return 0;
}
