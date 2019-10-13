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
		if (bitmapInfoHeaderSize != 40) {
			/*windowsBitmap�ｿｽ�ｿｽ40�ｿｽo�ｿｽC�ｿｽg�ｿｽﾅ定だ�ｿｽ�ｿｽ�ｿｽAOS/2�ｿｽﾌ場合�ｿｽ�ｿｽ12�ｿｽo�ｿｽC�ｿｽg�ｿｽ轤ｵ�ｿｽ�ｿｽ�ｿｽﾌで、OS/2�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ鼾��ｿｽﾌ擾ｿｽ�ｿｽ�ｿｽ*/
			/*TODO �ｿｽ�ｿｽ�ｿｽﾌ托ｿｽ�ｿｽ�ｿｽBitmap�ｿｽt�ｿｽ@�ｿｽC�ｿｽ�ｿｽ�ｿｽ�ｿｽm�ｿｽF�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾙゑｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ*/
			printf("This bitmap file is not windowsBitmap\n");
			exit(1);
		}

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

		/*�ｿｽ鞫懶ｿｽﾌ包ｿｽ�ｿｽA�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ謫ｾ*/
		int width = bitmapInfoHeader[4];
		int height = bitmapInfoHeader[8];

		/*1�ｿｽ�ｿｽf�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾌデ�ｿｽ[�ｿｽ^�ｿｽT�ｿｽC�ｿｽY�ｿｽ�ｿｽ謫ｾ*/
		short bitsPerPixel = bitmapInfoHeader[14];
		
		if (bitsPerPixel != 24) {
			/*�ｿｽt�ｿｽ@�ｿｽC�ｿｽ�ｿｽ�ｿｽ�ｿｽ24bit�ｿｽﾅなゑｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾛの擾ｿｽ�ｿｽ�ｿｽ*/
			printf("This file is not 24bits");
			exit(1);
		}
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