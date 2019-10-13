#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#define BITMAP_FILEHEADER_SIZE 14
#define FILE_TYPE_SIZE 2

int main(void) {

	FILE *file;
	int err = fopen_s(&file, "sampleImage.bmp", "r");

	if (err == 0) {
		/*ファイルの取得に成功した際の処理*/

		/*ファイルヘッダの情報を格納する領域を確保*/
		char bitmapFileHeader[BITMAP_FILEHEADER_SIZE];

		/*ファイルヘッダの情報を取得*/
		fread(&bitmapFileHeader, sizeof(char), BITMAP_FILEHEADER_SIZE, file);

		if (strncmp("BM", bitmapFileHeader, 2) != 0) {
			/*ファイルヘッダのファイルタイプがBMでなかった際の処理*/
			printf("This file is not BMP file\n");
			exit(1);
		}
		/*TODO ファイルが24bytes形式でない場合の処理*/

		/*情報ヘッダのサイズを取得*/
		unsigned char bitmapInfoHeaderSize = fgetc(file);

		/*ファイルの位置指定子を1バイト戻して、情報ヘッダの開始位置へ*/
		fseek(file, 1, SEEK_CUR);

		/*情報ヘッダの情報を格納する領域を確保*/
		char *bitmapInfoHeader = (char *)malloc(40);
		if (bitmapInfoHeader == NULL) {
			/*メモリの割当に失敗した場合の処理*/
			printf("Faild to allocate memory\n");
			exit(1);
		}

		/*情報ヘッダの情報を取得*/
		fread(bitmapInfoHeader, sizeof(char), bitmapInfoHeaderSize, file);

		printf("info:%c\n", bitmapInfoHeader[0]);

	}
	else if (err == ENOENT) {
		/*ファイルが存在しなかった際の処理*/
		printf("File is not exist\n");
		exit(1);
	}

	_fcloseall();

	printf("end");

	return 0;
}