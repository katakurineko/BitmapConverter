#include<stdio.h>

#include "bitmapInfoHeader.h"
#include "calc.h"

/*8bit形式windowsBitmapファイルの、画像データまでのオフセットサイズ
(ファイルヘッダ)14 + (情報ヘッダ)40 + (カラーパレッド)256*4 */
#define BF_OFF_BITS_VALUE 1078

void writeBitmapFileHeader(FILE* file,bitmapInfoHeader* pBIH) {

	/*ファイル形式の書き込み*/
	fwrite("BM", strlen("BM"), 1, file);

	/*画像データ部のサイズの計算*/
	unsigned long pictureDataSize = calcMultipleOf4(pBIH->biWidth) * pBIH->biHeight;

	/*ファイルサイズを計算して書き込み*/
	unsigned long bfSize = BF_OFF_BITS_VALUE + pictureDataSize;
	fwrite(&bfSize, sizeof(bfSize), 1, file);

	/*予約領域の書き込み*/
	signed short bfReserved1 = 0;
	fwrite(&bfReserved1, sizeof(bfReserved1), 1, file);
	signed short bfReserved2 = 0;
	fwrite(&bfReserved2, sizeof(bfReserved2), 1, file);

	/*ファイル先頭から画像データまでのオフセットの書き込み*/
	unsigned long bfOffBits = BF_OFF_BITS_VALUE;
	fwrite(&bfOffBits, sizeof(bfOffBits), 1, file);
}