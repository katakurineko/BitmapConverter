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
		/*�t�@�C���̎擾�ɐ��������ۂ̏���*/

		/*�t�@�C���w�b�_�̏���i�[����̈��m��*/
		char bitmapFileHeader[BITMAP_FILEHEADER_SIZE];

		fread(&bitmapFileHeader, sizeof(char), BITMAP_FILEHEADER_SIZE, file);

		if (strncmp("BM", bitmapFileHeader, 2) != 0) {
			/*�t�@�C���w�b�_�̃t�@�C���^�C�v��BM�łȂ������ۂ̏���*/
			printf("This file is not BMP file");
			exit(1);
		}
	}
	else if (err == ENOENT) {
		/*�t�@�C�������݂��Ȃ������ۂ̏���*/
		printf("File is not exist\n");
		exit(1);
	}

	_fcloseall();

	printf("end");

	return 0;
}