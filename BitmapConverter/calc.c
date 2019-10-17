#include"pixelDataRGB.h"


/*0���܂ޔC�ӂ̐��̐����́A���Ɍ����4�̔{����Ԃ��֐�.*/
unsigned long calcMultipleOf4(unsigned long x) {
	return ((x - 1) / 4 + 1) * 4;
}

/*RGB����P�x�ւ̕ϊ��W��*/
#define RED_COEFF 0.299
#define BLUE_COEFF 0.587
#define GREEN_COEFF 0.114

/*RGB����P�x���v�Z����֐�*/
unsigned char calcLuminance(struct pixelDataRGB *pixelData) {
	return GREEN_COEFF * pixelData->green + BLUE_COEFF * pixelData->blue + RED_COEFF * pixelData->red;
}