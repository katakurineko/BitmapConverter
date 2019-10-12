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

		/*ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½wï¿½bï¿½_ï¿½Ìï¿½ï¿½ï¿½iï¿½[ï¿½ï¿½ï¿½ï¿½Ìˆï¿½ï¿½mï¿½ï¿½*/
		char bitmapFileHeader[BITMAP_FILEHEADER_SIZE];

		fread(&bitmapFileHeader, sizeof(char), BITMAP_FILEHEADER_SIZE, file);

		if (strncmp("BM", bitmapFileHeader, 2) != 0) {
			/*ƒtƒ@ƒCƒ‹ƒwƒbƒ_‚Ìƒtƒ@ƒCƒ‹ƒ^ƒCƒv‚ªBM‚Å‚È‚©‚Á‚½Û‚Ìˆ—*/
			printf("This file is not BMP file");
			exit(1);
		}
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