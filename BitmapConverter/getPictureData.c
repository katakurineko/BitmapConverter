#include<stdio.h>
#include <stdlib.h>

#include "bitmapInfoHeader.h"

unsigned char* getPictureData(FILE* file,bitmapInfoHeader* pBIH) {

	/*�ϊ��O�̉摜�f�[�^�̃T�C�Y���v�Z*/
	unsigned long pictureDataSize = calcMultipleOf4(pBIH->biWidth * 3) * pBIH->biHeight;

	/*�摜�f�[�^�̏����i�[����̈�̊m��*/
	unsigned char *pictureData = (unsigned char *)malloc(pictureDataSize);
	memset(pictureData, 0x00, pictureDataSize);

	if (NULL == pictureData) {
		/*�������̊����Ɏ��s�����ۂ̏���*/
		printf("Faild to allocate memory\n");
		exit(1);
	}

	/*�摜�f�[�^�̏����擾*/
	fread(pictureData, pictureDataSize, 1, file);

	return pictureData;
}