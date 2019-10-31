#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#include<stdio.h>
#include<string.h>
#include<errno.h>

#include"strJoin.h"
#include"calc.h"
#include"pixelDataRGB.h"
#include"myBool.h"
#include"isBitmapFile.h"
#include"getBitmapInfoHeader.h"
#include "bitmapInfoHeader.h"
#include "writeColorPaletteGrayScale.h"

/*24bit�`������8bit�`���ɕϊ������t�@�C���̖��O�̑O�ɕt���镶����*/
#define ADD_FILE_NAME "convert_"

#define WINDOWS_BITMAP_FILE_SIZE 40

#define BI_PLANES_VALUE 1

#define BI_BIT_COUNT_VALUE 8

#define NOT_COMPRESSION 0

#define BI_X_PELS_PER_METER_VALUE 0
#define BI_Y_PELS_PER_METER_VALUE 0

/*8bit�`���̏ꍇ�̃p���b�g��*/
#define BI_CLR_USED_8BIT 256

/*�d�v�ȃp���b�h�̃C���f�b�N�X(0�͂��ׂďd�v�̈Ӗ�)*/
#define BI_CLR_IMPORTANT_VALUE 0

/*�J���[�p���b�h�̗\��̈�̒l*/
#define RGB_RESERVED 0

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
	bitmapInfoHeader BIH = *getBitmapInfoHeader(inputFile);
	bitmapInfoHeader* pBIH = &BIH;

	/*�摜�f�[�^�̏����擾*/
	unsigned char* pictureData = NULL;
	pictureData = getPictureData(inputFile, pBIH);

	fclose(inputFile);


	/*�t�@�C���w�b�_�̏�������*/
	writeBitmapFileHeader(outputFile, pBIH);

	/*���w�b�_�̏�������*/
	writeBitmapInfoHeader(outputFile, pBIH);

	/*�J���[�p���b�h�̏�������*/
	writeColorPaletteGrayScale(outputFile);


	unsigned long  width = pBIH->biWidth;
	long  height = pBIH->biHeight;

	/*�C���v�b�g�t�@�C���̉摜�f�[�^�̃p�f�B���O��*/
	unsigned char inputPaddingNum = calcMultipleOf4(width * 3) - width * 3;

	/*�A�E�g�v�b�g�t�@�C���̉摜�f�[�^�̃p�f�B���O��*/
	unsigned char outputPaddingNum = calcMultipleOf4(width) - width;

	/*�ϊ��O�摜�̉摜�f�[�^�i�[�p�\����*/
	pixelDataRGB inputFilePixelData;
	inputFilePixelData.red = 0x00;
	inputFilePixelData.green = 0x00;
	inputFilePixelData.blue = 0x00;

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
