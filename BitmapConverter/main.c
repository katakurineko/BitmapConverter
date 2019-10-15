#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include"strJoin.h"
#include"calc.h"


/*24bit�`������8bit�`���ɕϊ������t�@�C���̖��O�̑O�ɕt���镶����*/
#define ADD_FILE_NAME "convert_"

#define BITMAP_FILEHEADER_SIZE 14
#define FILE_TYPE_SIZE 2


int main(void) {

	FILE *preFile, *postFile;

	/*�ϊ�����t�@�C���̖��O*/
	char preFileName[] = "24sample.bmp";

	/*�ϊ���̃t�@�C����*/
	char *postFileName = strJoin(ADD_FILE_NAME, preFileName);

	int preFileErr = fopen_s(&preFile, preFileName, "r");
	int postFileErr = fopen_s(&postFile, postFileName, "w");
	

	if (preFileErr == 0) {
		/*�t�@�C���̎擾�ɐ��������ۂ̏���*/

		/*�t�@�C���w�b�_�̏����i�[����̈���m��*/
		char bitmapFileHeader[BITMAP_FILEHEADER_SIZE];

		/*�t�@�C���w�b�_�̏����擾*/
		fread(&bitmapFileHeader, sizeof(char), BITMAP_FILEHEADER_SIZE, preFile);

		if (strncmp("BM", bitmapFileHeader, 2) != 0) {
			/*�t�@�C���w�b�_�̃t�@�C���^�C�v��BM�łȂ������ۂ̏���*/
			printf("This file is not BMP file\n");
			exit(1);
		}



		/*���w�b�_�̃T�C�Y���擾*/
		unsigned char bitmapInfoHeaderSize = fgetc(preFile);
		if (bitmapInfoHeaderSize != 40) {
			/*windowsBitmap��40�o�C�g�Œ肾���AOS/2�̏ꍇ��12�o�C�g�炵���̂ŁAOS/2�������ꍇ�̏���*/
			/*TODO ���̑���Bitmap�t�@�C�����m�F�����ق�����������*/
			printf("This bitmap file is not windowsBitmap\n");
			exit(1);
		}

		/*�t�@�C���̈ʒu�w��q��1�o�C�g�߂��āA���w�b�_�̊J�n�ʒu��*/
		fseek(preFile, -1, SEEK_CUR);

		/*���w�b�_�̏����i�[����̈���m��*/
		char *bitmapInfoHeader = (char *)malloc(bitmapInfoHeaderSize);
		if (bitmapInfoHeader == NULL) {
			/*�������̊����Ɏ��s�����ۂ̏���*/
			printf("Faild to allocate memory\n");
			exit(1);
		}

		/*���w�b�_�̏����擾*/
		fread(bitmapInfoHeader, sizeof(char), bitmapInfoHeaderSize, preFile);

		/*�摜�̕��A�������擾*/
		unsigned long width = bitmapInfoHeader[4];
		unsigned long height = bitmapInfoHeader[8];

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



		/*�摜�̕���4�̔{���ɕϊ�*/
		unsigned long widthMultipleOf4 = calcMultipleOf4(width);

		/*�摜�f�[�^�̃T�C�Y���v�Z*/
		unsigned char pictureDataSize = (widthMultipleOf4 * height) * 3;

		/*�摜�f�[�^�̏����i�[����̈���m��*/
		unsigned char *pictureData = (unsigned char *)malloc(pictureDataSize);
		if (pictureData == NULL) {
			/*�������̊����Ɏ��s�����ۂ̏���*/
			printf("Faild to allocate memory\n");
			exit(1);
		}

		/*���w�b�_�̏����擾*/
		fread(pictureData, sizeof(char), pictureDataSize, preFile);


		for (int i = 0; i < pictureDataSize; i++) {
			printf("%lx,%lx,%lx\n", pictureData[i * 3], pictureData[i * 3 + 1], pictureData[i * 3 + 2]);
		}
	}
	else if (preFileErr == ENOENT) {
		/*�t�@�C�������݂��Ȃ������ۂ̏���*/
		printf("File is not exist\n");
		exit(1);
	}

	_fcloseall();


	printf("end");

	return 0;
}
