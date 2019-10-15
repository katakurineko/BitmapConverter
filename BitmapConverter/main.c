#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include"strJoin.h"
#include"calc.h"


/*24bit形式から8bit形式に変換したファイルの名前の前に付ける文字列*/
#define ADD_FILE_NAME "convert_"

#define BITMAP_FILEHEADER_SIZE 14
#define FILE_TYPE_SIZE 2


int main(void) {

	FILE *preFile, *postFile;

	/*変換するファイルの名前*/
	char preFileName[] = "24sample.bmp";

	/*変換後のファイル名*/
	char *postFileName = strJoin(ADD_FILE_NAME, preFileName);

	int preFileErr = fopen_s(&preFile, preFileName, "r");
	int postFileErr = fopen_s(&postFile, postFileName, "w");
	

	if (preFileErr == 0) {
		/*ファイルの取得に成功した際の処理*/

		/*ファイルヘッダの情報を格納する領域を確保*/
		char bitmapFileHeader[BITMAP_FILEHEADER_SIZE];

		/*ファイルヘッダの情報を取得*/
		fread(&bitmapFileHeader, sizeof(char), BITMAP_FILEHEADER_SIZE, preFile);

		if (strncmp("BM", bitmapFileHeader, 2) != 0) {
			/*ファイルヘッダのファイルタイプがBMでなかった際の処理*/
			printf("This file is not BMP file\n");
			exit(1);
		}



		/*情報ヘッダのサイズを取得*/
		unsigned char bitmapInfoHeaderSize = fgetc(preFile);
		if (bitmapInfoHeaderSize != 40) {
			/*windowsBitmapは40バイト固定だが、OS/2の場合は12バイトらしいので、OS/2だった場合の処理*/
			/*TODO その他のBitmapファイルも確認したほうがいいかも*/
			printf("This bitmap file is not windowsBitmap\n");
			exit(1);
		}

		/*ファイルの位置指定子を1バイト戻して、情報ヘッダの開始位置へ*/
		fseek(preFile, -1, SEEK_CUR);

		/*情報ヘッダの情報を格納する領域を確保*/
		char *bitmapInfoHeader = (char *)malloc(bitmapInfoHeaderSize);
		if (bitmapInfoHeader == NULL) {
			/*メモリの割当に失敗した際の処理*/
			printf("Faild to allocate memory\n");
			exit(1);
		}

		/*情報ヘッダの情報を取得*/
		fread(bitmapInfoHeader, sizeof(char), bitmapInfoHeaderSize, preFile);

		/*画像の幅、高さを取得*/
		unsigned long width = bitmapInfoHeader[4];
		unsigned long height = bitmapInfoHeader[8];

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



		/*画像の幅を4の倍数に変換*/
		unsigned long widthMultipleOf4 = calcMultipleOf4(width);

		/*画像データのサイズを計算*/
		unsigned char pictureDataSize = (widthMultipleOf4 * height) * 3;

		/*画像データの情報を格納する領域を確保*/
		unsigned char *pictureData = (unsigned char *)malloc(pictureDataSize);
		if (pictureData == NULL) {
			/*メモリの割当に失敗した際の処理*/
			printf("Faild to allocate memory\n");
			exit(1);
		}

		/*情報ヘッダの情報を取得*/
		fread(pictureData, sizeof(char), pictureDataSize, preFile);


		for (int i = 0; i < pictureDataSize; i++) {
			printf("%lx,%lx,%lx\n", pictureData[i * 3], pictureData[i * 3 + 1], pictureData[i * 3 + 2]);
		}
	}
	else if (preFileErr == ENOENT) {
		/*ファイルが存在しなかった際の処理*/
		printf("File is not exist\n");
		exit(1);
	}

	_fcloseall();


	printf("end");

	return 0;
}
