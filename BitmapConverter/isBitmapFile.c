#include<stdio.h>
#include <string.h>

#include "myBool.h"

#define BITMAP_FILEHEADER_REGION_SIZE 14

#define BF_TYPE_REGION_SIZE 2

char isBitmapFile(FILE* file) {

	/*ファイルヘッダの情報を格納する領域*/
	char bitmapFileHeader[BITMAP_FILEHEADER_REGION_SIZE] = { 0 };

	/*ファイルヘッダの情報を取得*/
	fread(&bitmapFileHeader, sizeof(bitmapFileHeader), 1, file);

	/*ファイル形式の領域(2バイト（2文字）+1バイト(ヌル文字分))を確保する.*/
	char bfType[BF_TYPE_REGION_SIZE + sizeof(char)];

	/*ファイル形式を取得する*/
	snprintf(bfType, BF_TYPE_REGION_SIZE + sizeof(char), "%s", bitmapFileHeader);

	if (0 != strcmp("BM", bfType)) {
		/*ファイルヘッダのファイルタイプがBMでなかった際の処理*/
		return FALSE;
	}

	return TRUE;
}