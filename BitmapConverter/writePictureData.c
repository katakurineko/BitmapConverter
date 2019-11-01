#include<stdio.h>

#include"pixelDataRGB.h"

#include "bitmapInfoHeader.h"

void writePictureData(FILE* file, bitmapInfoHeader* pBIH, pixelDataRGB* pictureData) {
	unsigned long  width = pBIH->biWidth;
	unsigned long  height = abs(pBIH->biHeight);

	/*8bitビットマップファイルの画像データのパディング数*/
	unsigned char paddingNum = calcMultipleOf4(width) - width;

	unsigned long count = 0;
	for (unsigned long h = 0; h < height; h++) {


		for (unsigned long w = 0; w < width; w++) {
			unsigned char luminance = calcLuminance(pictureData + count);
			fwrite(&luminance, sizeof(luminance), 1, file);

			/*プログレス表示*/
			printf("\r[%lu/%lu](%lu%%)", count + 1, height * width, (count + 1) * 100 / (height * width));
			
			count++;
		}

		/*パディングの書き込み*/
		for (unsigned char padd = 0; padd < paddingNum; padd++) {
			fputc(0x00, file);
		}
	}
}