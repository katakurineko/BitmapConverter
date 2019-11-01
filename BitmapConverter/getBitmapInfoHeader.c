#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmapInfoHeader.h"

bitmapInfoHeader* getBitmapInfoHeader(FILE* pFile) {

	bitmapInfoHeader* pBIH = (bitmapInfoHeader*)malloc(sizeof(bitmapInfoHeader));

	if (NULL == pBIH) {
		/*�������̊����Ɏ��s�����ۂ̏���*/
		printf("Faild to allocate memory\n");
		exit(1);
	}

	/*�Ԃ�l�Ƃ��ĕԂ������\����bitmapInfoHeader��������*/
	pBIH->biSize = 0;
	pBIH->biWidth = 0;
	pBIH->biHeight = 0;
	pBIH->biPlanes = 1;
	pBIH->biBitCount = 0;
	pBIH->biCompression = 0;
	pBIH->biSizeImage = 0;
	pBIH->biXPelsPerMeter = 0;
	pBIH->biYPelsPerMeter = 0;
	pBIH->biClrUsed = 0;
	pBIH->biClrImportant = 0;

	/*���w�b�_�̃T�C�Y���擾*/
	unsigned char bitmapInfoHeaderSize = fgetc(pFile);

	/*windowsBitmap��40�o�C�g�����AOS/2�̏ꍇ��12�o�C�g�炵���̂ŁA
	windowsBitmap���ǂ����𔻒�*/
	if (WINDOWS_BITMAP_FILE_SIZE != bitmapInfoHeaderSize) {
		printf("This bitmap file is not windowsBitmap\n");
		exit(1);
	}

	/*�t�@�C���̈ʒu�w��q��1�o�C�g�߂��āA���w�b�_�̊J�n�ʒu��*/
	fseek(pFile, -1, SEEK_CUR);

	/*���w�b�_�̏����i�[����̈�̊m��*/
	char* bitmapInfoHeader = (char *)malloc(bitmapInfoHeaderSize);
	if (NULL == bitmapInfoHeader) {
		/*�������̊����Ɏ��s�����ۂ̏���*/
		printf("Faild to allocate memory\n");
		exit(1);
	}
	memset(bitmapInfoHeader, 0x00, bitmapInfoHeaderSize);

	/*���w�b�_�̏����擾*/
	fread(bitmapInfoHeader, bitmapInfoHeaderSize, 1, pFile);

	/*�摜�̕��A�������擾*/
	memcpy(&pBIH->biWidth, &bitmapInfoHeader[4], sizeof(pBIH->biWidth));
	memcpy(&pBIH->biHeight, &bitmapInfoHeader[8], sizeof(pBIH->biHeight));

	/*1��f������̃f�[�^�T�C�Y���擾*/
	memcpy(&pBIH->biBitCount, &bitmapInfoHeader[14], sizeof(pBIH->biBitCount));

	/*24bit�`�����ǂ����̔���*/
	if (24 != pBIH->biBitCount) {
		printf("This file is not 24bits");
		exit(1);
	}

	/*���k�`�����擾*/
	memcpy(&pBIH->biCompression, &bitmapInfoHeader[16], sizeof(pBIH->biCompression));

	/*�t�@�C�������k����Ă��Ȃ����̔���*/
	if (NOT_COMPRESSION != pBIH->biCompression) {
		printf("File is compressed");
		exit(1);
	}


	free(bitmapInfoHeader);

	return pBIH;
}