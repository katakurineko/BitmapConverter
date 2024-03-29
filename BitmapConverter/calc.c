#include"pixelDataRGB.h"


/*0を含む任意の正の整数以上で最小の、4の倍数を返す関数.*/
unsigned long calcMultipleOf4(unsigned long x) {
	return ((x - 1) / 4 + 1) * 4;
}

/*RGBから輝度への変換係数*/
#define RED_COEFF 0.299
#define GREEN_COEFF 0.587
#define BLUE_COEFF 0.114

/*RGBから輝度を計算する関数*/
unsigned char calcLuminance(struct pixelDataRGB *pixelData) {
	return GREEN_COEFF * pixelData->green + BLUE_COEFF * pixelData->blue + RED_COEFF * pixelData->red;
}