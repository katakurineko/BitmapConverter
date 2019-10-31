#include<stdio.h>

#include "bitmapInfoHeader.h"
#include "calc.h"

void writeBitmapInfoHeader(FILE* file, bitmapInfoHeader* pBIH) {
	/*情報ヘッダのサイズの書き込み*/
	unsigned long biSize = WINDOWS_BITMAP_FILE_SIZE;
	fwrite(&biSize, sizeof(biSize), 1, file);

	/*画像の幅と高さを書き込み*/
	fwrite(&(pBIH->biWidth), sizeof(pBIH->biWidth), 1, file);
	fwrite(&(pBIH->biHeight), sizeof(pBIH->biHeight), 1, file);

	/*プレーン数を書き込み*/
	unsigned short biPlanes = BI_PLANES_VALUE;
	fwrite(&biPlanes, sizeof(biPlanes), 1, file);

	/*1画素あたりのデータサイズを書き込み*/
	unsigned short biBitCount = BI_BIT_COUNT_VALUE;
	fwrite(&biBitCount, sizeof(biBitCount), 1, file);

	/*圧縮形式を書き込み*/
	unsigned long biCompression = NOT_COMPRESSION;
	fwrite(&biCompression, sizeof(biCompression), 1, file);

	/*画像データ部のサイズの計算*/
	unsigned long pictureDataSize = calcMultipleOf4(pBIH->biWidth) * pBIH->biHeight;
	
	/*画像データ部のサイズを書き込み*/
	fwrite(&pictureDataSize, sizeof(pictureDataSize), 1, file);

	/*解像度を書き込み*/
	unsigned long biXPixPerMeter = BI_X_PELS_PER_METER_VALUE;
	unsigned long biYPixPerMeter = BI_Y_PELS_PER_METER_VALUE;
	fwrite(&biXPixPerMeter, sizeof(biXPixPerMeter), 1, file);
	fwrite(&biYPixPerMeter, sizeof(biYPixPerMeter), 1, file);

	/*パレット数の書き込み*/
	unsigned long biClrUsed = BI_CLR_USED_8BIT;
	fwrite(&biClrUsed, sizeof(biClrUsed), 1, file);

	/*重要なパレットのインデックスを書き込み*/
	unsigned long biClrImportant = BI_CLR_IMPORTANT_VALUE;
	fwrite(&biClrImportant, sizeof(biClrImportant), 1, file);
}