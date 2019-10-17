#include"pixelDataRGB.h"


/*0‚ğŠÜ‚Ş”CˆÓ‚Ì³‚Ì®”‚ÌAŸ‚ÉŒ»‚ê‚é4‚Ì”{”‚ğ•Ô‚·ŠÖ”.*/
unsigned long calcMultipleOf4(unsigned long x) {
	return ((x - 1) / 4 + 1) * 4;
}

/*RGB‚©‚ç‹P“x‚Ö‚Ì•ÏŠ·ŒW”*/
#define RED_COEFF 0.299
#define BLUE_COEFF 0.587
#define GREEN_COEFF 0.114

/*RGB‚©‚ç‹P“x‚ğŒvZ‚·‚éŠÖ”*/
unsigned char calcLuminance(struct pixelDataRGB *pixelData) {
	return GREEN_COEFF * pixelData->green + BLUE_COEFF * pixelData->blue + RED_COEFF * pixelData->red;
}