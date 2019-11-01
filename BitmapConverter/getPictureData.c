#include<stdio.h>
#include <stdlib.h>

#include "bitmapInfoHeader.h"
#include"pixelDataRGB.h"

pixelDataRGB* getPictureData(FILE* pFile,bitmapInfoHeader* pBIH) {

	/*�s�N�Z�������v�Z*/
	unsigned long pixelNum = pBIH->biWidth * abs(pBIH->biHeight);

	/*�摜�f�[�^�̏����i�[����̈�̊m��*/
	pixelDataRGB* pictureData = (pixelDataRGB*)malloc(sizeof(pixelDataRGB) * pixelNum);
	memset(pictureData, 0x00, pixelNum * sizeof(*pictureData));

	if (NULL == pictureData) {
		/*�������̊����Ɏ��s�����ۂ̏���*/
		printf("Faild to allocate memory\n");
		exit(1);
	}

	/*�摜�f�[�^�̏����擾*/
	//fread(pictureData, pixelNum * sizeof(*pictureData), 1, file);

	/*��s���Ƃ̃p�f�B���O�̐�*/
	unsigned char paddingNum = calcMultipleOf4(pBIH->biWidth * 3) - pBIH->biWidth * 3;

	unsigned long count = 0;
	for (unsigned long h = 0; h < pBIH->biHeight; h++) {
		for (unsigned long w = 0; w < pBIH->biWidth; w++) {
			fread(pictureData + count, sizeof(pixelDataRGB), 1, pFile);
			count++;
		}
		/*�t�@�C���̈ʒu�w��q���p�f�B���O�̕��������炷*/
		fseek(pFile, paddingNum, SEEK_CUR);
	}

	return pictureData;
}