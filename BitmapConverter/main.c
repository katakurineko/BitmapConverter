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

		/*情報ヘッダのサイズを取得*/
		unsigned char bitmapInfoHeaderSize = fgetc(file);
		if (bitmapInfoHeaderSize != 40) {
			/*windowsBitmapは40バイト固定だが、OS/2の場合は12バイトらしいので、OS/2だった場合の処理*/
			/*TODO その他のBitmapファイルも確認したほうがいいかも*/
			printf("This bitmap file is not windowsBitmap\n");
			exit(1);
		}

		/*ファイルの位置指定子を1バイト戻して、情報ヘッダの開始位置へ*/
		fseek(file, 1, SEEK_CUR);

		/*情報ヘッダの情報を格納する領域を確保*/
		char *bitmapInfoHeader = (char *)malloc(40);
		if (bitmapInfoHeader == NULL) {
			/*メモリの割当に失敗した際の処理*/
			printf("Faild to allocate memory\n");
			exit(1);
		}

		/*情報ヘッダの情報を取得*/
		fread(bitmapInfoHeader, sizeof(char), bitmapInfoHeaderSize, file);

		/*画像の幅、高さを取得*/
		int width = bitmapInfoHeader[4];
		int height = bitmapInfoHeader[8];

		/*1画素あたりのデータサイズを取得*/
		short bitsPerPixel = bitmapInfoHeader[14];
		
		if (bitsPerPixel != 24) {
			/*ファイルが24bitでなかった際の処理*/
			printf("This file is not 24bits");
			exit(1);
		}

		/*圧縮形式を取得*/
		int compression = bitmapInfoHeader[16];

		if (compression != 0) {
			/*ファイルが圧縮されている際の処理*/
			printf("File is compressed");
			exit(1);
		}

		printf("%d\n", compression);
		
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