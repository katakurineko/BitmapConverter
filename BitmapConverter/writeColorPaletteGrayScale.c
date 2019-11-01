#include<stdio.h>

/*8bit形式の場合のパレット数*/
#define BI_CLR_USED_8BIT 256

#define COLOR_PALETTE_RESERVED 0

void writeColorPaletteGrayScale(FILE* pFile) {

	unsigned char rgbColorPalette = 0;
	unsigned char colorPaletteReserved = COLOR_PALETTE_RESERVED;

	for (int i = 0; i < BI_CLR_USED_8BIT; i++) {
		for (int j = 0; j < 3; j++) {
			fwrite(&rgbColorPalette, sizeof(rgbColorPalette), 1, pFile);
		}
		fwrite(&colorPaletteReserved, sizeof(colorPaletteReserved), 1, pFile);
		rgbColorPalette++;
	}

 }