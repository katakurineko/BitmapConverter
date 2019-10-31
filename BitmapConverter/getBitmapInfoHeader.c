#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmapInfoHeader.h"

bitmapInfoHeader* getBitmapInfoHeader(FILE* file) {

	/*�Ԃ�l�Ƃ��ĕԂ������\����bitmapInfoHeader��������*/
	bitmapInfoHeader BIH = {.biSize = 0, .biWidth =0,.biHeight =0,
		.biPlanes =1,.biBitCount =0,.biCompression =0,.biSizeImage = 0,
		.biXPelsPerMeter =0,.biYPelsPerMeter =0,.biClrUsed =0,.biClrImportant =0};

	/*���w�b�_�̃T�C�Y���擾*/
	unsigned char bitmapInfoHeaderSize = fgetc(file);

	/*windowsBitmap��40�o�C�g�����AOS/2�̏ꍇ��12�o�C�g�炵���̂ŁA
	windowsBitmap���ǂ����𔻒�*/
	if (WINDOWS_BITMAP_FILE_SIZE != bitmapInfoHeaderSize) {
		printf("This bitmap file is not windowsBitmap\n");
		exit(1);
	}

	/*�t�@�C���̈ʒu�w��q��1�o�C�g�߂��āA���w�b�_�̊J�n�ʒu��*/
	fseek(file, -1, SEEK_CUR);

	/*���w�b�_�̏����i�[����̈�̊m��*/
	char* bitmapInfoHeader = (char *)malloc(bitmapInfoHeaderSize);
	memset(bitmapInfoHeader, 0x00, bitmapInfoHeaderSize);
	if (NULL == bitmapInfoHeader) {
		/*�������̊����Ɏ��s�����ۂ̏���*/
		printf("Faild to allocate memory\n");
		exit(1);
	}

	/*���w�b�_�̏����擾*/
	fread(bitmapInfoHeader, bitmapInfoHeaderSize, 1, file);

	/*�摜�̕��A�������擾*/
	memcpy(&BIH.biWidth, &bitmapInfoHeader[4], sizeof(BIH.biWidth));
	memcpy(&BIH.biHeight, &bitmapInfoHeader[8], sizeof(BIH.biHeight));

	/*1��f������̃f�[�^�T�C�Y���擾*/
	memcpy(&BIH.biBitCount, &bitmapInfoHeader[14], sizeof(BIH.biBitCount));

	/*24bit�`�����ǂ����̔���*/
	if (24 != BIH.biBitCount) {
		printf("This file is not 24bits");
		exit(1);
	}

	/*���k�`�����擾*/
	memcpy(&BIH.biCompression, &bitmapInfoHeader[16], sizeof(BIH.biCompression));

	/*�t�@�C�������k����Ă��Ȃ����̔���*/
	if (NOT_COMPRESSION != BIH.biCompression) {
		printf("File is compressed");
		exit(1);
	}


	free(bitmapInfoHeader);

	return &BIH;
}