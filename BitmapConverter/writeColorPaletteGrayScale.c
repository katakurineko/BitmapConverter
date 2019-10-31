#include<stdio.h>

/*8bit�`���̏ꍇ�̃p���b�g��*/
#define BI_CLR_USED_8BIT 256

#define COLOR_PALETTE_RESERVED 0

void writeColorPaletteGrayScale(FILE* file) {

	unsigned char rgbColorPalette = 0;
	unsigned char colorPaletteReserved = COLOR_PALETTE_RESERVED;

	for (int i = 0; i < BI_CLR_USED_8BIT; i++) {
		for (int j = 0; j < 3; j++) {
			fwrite(&rgbColorPalette, sizeof(rgbColorPalette), 1, file);
		}
		fwrite(&colorPaletteReserved, sizeof(colorPaletteReserved), 1, file);
		rgbColorPalette++;
	}

 }