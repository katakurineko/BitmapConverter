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

	/*�t�@�C���̈ʒu�w��q��1�o�C�g�߂��āA���w�b�_�̊J�n�ʒu��*/
	fseek(pFile, -1, SEEK_CUR);

	/*���w�b�_�̏����擾*/
	fread(pBIH, bitmapInfoHeaderSize, 1, pFile);

	/*windowsBitmap��40�o�C�g�����AOS/2�̏ꍇ��12�o�C�g�炵���̂ŁA
	windowsBitmap���ǂ����𔻒�*/
	if (WINDOWS_BITMAP_FILE_SIZE != pBIH->biSize) {
		printf("This bitmap file is not windowsBitmap\n");
		exit(1);
	}

	/*24bit�`�����ǂ����̔���*/
	if (24 != pBIH->biBitCount) {
		printf("This file is not 24bits");
		exit(1);
	}

	/*�t�@�C�������k����Ă��Ȃ����̔���*/
	if (NOT_COMPRESSION != pBIH->biCompression) {
		printf("File is compressed");
		exit(1);
	}

	return pBIH;
}