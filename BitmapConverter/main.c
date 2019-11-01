#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#include<stdio.h>
#include<string.h>
#include<errno.h>

/*����bool*/
#include"myBool.h"

/*����\����*/
#include"pixelDataRGB.h"
#include "bitmapInfoHeader.h"

/*����֐�*/
#include"strJoin.h"
#include"calc.h"
#include"isBitmapFile.h"
#include"getBitmapInfoHeader.h"
#include "getPictureData.h"
#include "writeColorPaletteGrayScale.h"
#include "writeBitmapFileHeader.h"
#include "writeBitmapInfoHeader.h"

/*24bit�`������8bit�`���ɕϊ������t�@�C���̖��O�̑O�ɕt���镶����*/
#define ADD_FILE_NAME "convert_"

int main(void) {
	FILE* inputFile = NULL;
	FILE* outputFile = NULL;

	/*�ϊ�����t�@�C���̖��O*/
	char inputFileName[] = "24sample2.bmp";

	/*�ϊ���̃t�@�C����*/
	char* outputFileName = strJoin(ADD_FILE_NAME, inputFileName);

	inputFile = fopen(inputFileName, "rb");
	if (NULL == inputFile) {
		/*�ϊ��O�̃t�@�C�����J���̂Ɏ��s�����ꍇ�̏���*/
		printf("Failed to open %s\n", inputFileName);
		exit(1);
	}

	outputFile = fopen(outputFileName, "wb");


	/*�t�@�C���w�b�_�̃t�@�C���^�C�v��BM���ǂ����̔���*/
	if (!isBitmapFile(inputFile)) {
		printf("This file is not BMP file\n");
		exit(1);
	}

	/*���w�b�_�̏����擾���\���̂�*/
	bitmapInfoHeader* pBIH = getBitmapInfoHeader(inputFile);

	/*�摜�f�[�^�̏����擾*/
	unsigned char* pictureData = getPictureData(inputFile, pBIH);

	fclose(inputFile);


	/*�t�@�C���w�b�_�̏�������*/
	writeBitmapFileHeader(outputFile, pBIH);

	/*���w�b�_�̏�������*/
	writeBitmapInfoHeader(outputFile, pBIH);

	/*�J���[�p���b�h�̏�������*/
	writeColorPaletteGrayScale(outputFile);


	unsigned long  width = pBIH->biWidth;
	unsigned long  height = abs(pBIH->biHeight);

	free(pBIH);

	/*�C���v�b�g�t�@�C���̉摜�f�[�^�̃p�f�B���O��*/
	unsigned char inputPaddingNum = calcMultipleOf4(width * 3) - width * 3;

	/*�A�E�g�v�b�g�t�@�C���̉摜�f�[�^�̃p�f�B���O��*/
	unsigned char outputPaddingNum = calcMultipleOf4(width) - width;

	/*�ϊ��O�摜�̉摜�f�[�^�i�[�p�\����*/
	pixelDataRGB inputFilePixelData = {.red = 0,.green = 0,.blue = 0};

	/*�C���v�b�g�t�@�C���̉摜�f�[�^�́A��s������̃o�C�g��*/
	unsigned long bytesNumInRowOfInputFile = width * 3 + inputPaddingNum;

	for (unsigned long h = 0; h < height; h++) {

		/*�v���O���X�\��*/
		printf("\r[%lu/%lu](%lu%%)", h + 1, height, (h + 1) * 100 / height);

		for (unsigned long w = 0; w < width; w++) {
			inputFilePixelData.blue = pictureData[w * 3 + bytesNumInRowOfInputFile * h];
			inputFilePixelData.green = pictureData[w * 3 + 1 + bytesNumInRowOfInputFile * h];
			inputFilePixelData.red = pictureData[w * 3 + 2 + bytesNumInRowOfInputFile * h];

			unsigned char luminance = calcLuminance(&inputFilePixelData);
			fwrite(&luminance, sizeof(luminance), 1, outputFile);
		}

		/*�p�f�B���O�̏�������*/
		for (unsigned char padd = 0; padd < outputPaddingNum; padd++) {
			fputc(0x00, outputFile);
		}
	}

	free(pictureData);
	fclose(outputFile);

	printf("\n\nConversion succeeded.\n\nConverted file name is %s\n", outputFileName);

	/*�֐�strJoin����malloc���g�p���Ă���̂ŁA�������J��*/
	free(outputFileName);

	/*���������[�N���o*/
	_CrtDumpMemoryLeaks();

	return 0;
}
