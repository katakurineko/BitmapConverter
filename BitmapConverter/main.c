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
		/*ƒtƒ@ƒCƒ‹‚Ìæ“¾‚É¬Œ÷‚µ‚½Û‚Ìˆ—*/

		/*ƒtƒ@ƒCƒ‹ƒwƒbƒ_‚Ìî•ñ‚ğŠi”[‚·‚é—Ìˆæ‚ğŠm•Û*/
		char bitmapFileHeader[BITMAP_FILEHEADER_SIZE];

		/*ƒtƒ@ƒCƒ‹ƒwƒbƒ_‚Ìî•ñ‚ğæ“¾*/
		fread(&bitmapFileHeader, sizeof(char), BITMAP_FILEHEADER_SIZE, file);

		if (strncmp("BM", bitmapFileHeader, 2) != 0) {
			/*ƒtƒ@ƒCƒ‹ƒwƒbƒ_‚Ìƒtƒ@ƒCƒ‹ƒ^ƒCƒv‚ªBM‚Å‚È‚©‚Á‚½Û‚Ìˆ—*/
			printf("This file is not BMP file\n");
			exit(1);
		}
		/*TODO ƒtƒ@ƒCƒ‹‚ª24bytesŒ`®‚Å‚È‚¢ê‡‚Ìˆ—*/

		/*î•ñƒwƒbƒ_‚ÌƒTƒCƒY‚ğæ“¾*/
		unsigned char bitmapInfoHeaderSize = fgetc(file);
		if (bitmapInfoHeaderSize != 40) {
			/*windowsBitmapï¿½ï¿½40ï¿½oï¿½Cï¿½gï¿½Å’è‚¾ï¿½ï¿½ï¿½AOS/2ï¿½Ìê‡ï¿½ï¿½12ï¿½oï¿½Cï¿½gï¿½ç‚µï¿½ï¿½ï¿½Ì‚ÅAOS/2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê‡ï¿½Ìï¿½ï¿½ï¿½*/
			/*TODO ï¿½ï¿½ï¿½Ì‘ï¿½ï¿½ï¿½Bitmapï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½ï¿½ï¿½Ù‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½*/
			printf("This bitmap file is not windowsBitmap\n");
			exit(1);
		}

		/*ƒtƒ@ƒCƒ‹‚ÌˆÊ’uw’èq‚ğ1ƒoƒCƒg–ß‚µ‚ÄAî•ñƒwƒbƒ_‚ÌŠJnˆÊ’u‚Ö*/
		fseek(file, 1, SEEK_CUR);

		/*î•ñƒwƒbƒ_‚Ìî•ñ‚ğŠi”[‚·‚é—Ìˆæ‚ğŠm•Û*/
		char *bitmapInfoHeader = (char *)malloc(40);
		if (bitmapInfoHeader == NULL) {
			/*ƒƒ‚ƒŠ‚ÌŠ„“–‚É¸”s‚µ‚½ê‡‚Ìˆ—*/
			printf("Faild to allocate memory\n");
			exit(1);
		}

		/*î•ñƒwƒbƒ_‚Ìî•ñ‚ğæ“¾*/
		fread(bitmapInfoHeader, sizeof(char), bitmapInfoHeaderSize, file);

		/*ï¿½æ‘œï¿½Ì•ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½æ“¾*/
		int width = bitmapInfoHeader[4];
		int height = bitmapInfoHeader[8];

	}
	else if (err == ENOENT) {
		/*ƒtƒ@ƒCƒ‹‚ª‘¶İ‚µ‚È‚©‚Á‚½Û‚Ìˆ—*/
		printf("File is not exist\n");
		exit(1);
	}

	_fcloseall();

	printf("end");

	return 0;
}