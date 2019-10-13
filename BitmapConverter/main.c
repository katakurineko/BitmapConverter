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

		/*�t�@�C���w�b�_�̏����i�[����̈���m��*/
		char bitmapFileHeader[BITMAP_FILEHEADER_SIZE];

		/*�t�@�C���w�b�_�̏����擾*/
		fread(&bitmapFileHeader, sizeof(char), BITMAP_FILEHEADER_SIZE, file);

		if (strncmp("BM", bitmapFileHeader, 2) != 0) {
			/*�t�@�C���w�b�_�̃t�@�C���^�C�v��BM�łȂ������ۂ̏���*/
			printf("This file is not BMP file\n");
			exit(1);
		}

		/*���w�b�_�̃T�C�Y���擾*/
		unsigned char bitmapInfoHeaderSize = fgetc(file);
		if (bitmapInfoHeaderSize != 40) {
			/*windowsBitmap��40�o�C�g�Œ肾���AOS/2�̏ꍇ��12�o�C�g�炵���̂ŁAOS/2�������ꍇ�̏���*/
			/*TODO ���̑���Bitmap�t�@�C�����m�F�����ق�����������*/
			printf("This bitmap file is not windowsBitmap\n");
			exit(1);
		}

		/*�t�@�C���̈ʒu�w��q��1�o�C�g�߂��āA���w�b�_�̊J�n�ʒu��*/
		fseek(file, 1, SEEK_CUR);

		/*���w�b�_�̏����i�[����̈���m��*/
		char *bitmapInfoHeader = (char *)malloc(40);
		if (bitmapInfoHeader == NULL) {
			/*�������̊����Ɏ��s�����ۂ̏���*/
			printf("Faild to allocate memory\n");
			exit(1);
		}

		/*���w�b�_�̏����擾*/
		fread(bitmapInfoHeader, sizeof(char), bitmapInfoHeaderSize, file);

		/*�摜�̕��A�������擾*/
		int width = bitmapInfoHeader[4];
		int height = bitmapInfoHeader[8];

		/*1��f������̃f�[�^�T�C�Y���擾*/
		short bitsPerPixel = bitmapInfoHeader[14];
		
		if (bitsPerPixel != 24) {
			/*�t�@�C����24bit�łȂ������ۂ̏���*/
			printf("This file is not 24bits");
			exit(1);
		}

		/*���k�`�����擾*/
		int compression = bitmapInfoHeader[16];

		if (compression != 0) {
			/*�t�@�C�������k����Ă���ۂ̏���*/
			printf("File is compressed");
			exit(1);
		}

		printf("%d\n", compression);
		
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