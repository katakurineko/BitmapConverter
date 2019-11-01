#include<stdio.h>

/*����\����*/
#include"pixelDataRGB.h"
#include "bitmapInfoHeader.h"

#include"isBitmapFile.h"
#include"getBitmapInfoHeader.h"
#include "getPictureData.h"
#include "writeColorPaletteGrayScale.h"
#include "writeBitmapFileHeader.h"
#include "writeBitmapInfoHeader.h"
#include "writePictureData.h"


void convertBitmap24bitTo8bit(FILE* pInputFile, FILE* pOutputFile) {

	/*�t�@�C���w�b�_�̃t�@�C���^�C�v��BM���ǂ����̔���*/
	if (!isBitmapFile(pInputFile)) {
		printf("This file is not BMP file\n");
		exit(1);
	}

	/*���w�b�_�̏����擾���\���̂�*/
	bitmapInfoHeader* pBIH = getBitmapInfoHeader(pInputFile);

	/*�摜�f�[�^�̏����擾*/
	pixelDataRGB* pPictureData = getPictureData(pInputFile, pBIH);


	/*�t�@�C���w�b�_�̏�������*/
	writeBitmapFileHeader(pOutputFile, pBIH);

	/*���w�b�_�̏�������*/
	writeBitmapInfoHeader(pOutputFile, pBIH);

	/*�J���[�p���b�h�̏�������*/
	writeColorPaletteGrayScale(pOutputFile);

	/*�摜�f�[�^�̕ϊ��Ə�������*/
	writePictureData(pOutputFile, pBIH, pPictureData);


	free(pBIH);
	free(pPictureData);
}