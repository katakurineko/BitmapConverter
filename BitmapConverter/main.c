#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#include<stdio.h>

/*����֐�*/
#include"strJoin.h"
#include"convertBitmap24bitTo8bit.h"

/*24bit�`������8bit�`���ɕϊ������t�@�C���̖��O�̑O�ɕt���镶����*/
#define ADD_FILE_NAME "convert_"

/*�ϊ�����t�@�C���̖��O*/
#define INPUT_FILE_NAME "24sample2.bmp"

int main(void) {
	FILE* pInputFile = NULL;
	FILE* pOutputFile = NULL;

	/*�ϊ�����t�@�C���̖��O*/
	char inputFileName[] = INPUT_FILE_NAME;

	/*�ϊ���̃t�@�C����*/
	char* outputFileName = strJoin(ADD_FILE_NAME, inputFileName);

	pInputFile = fopen(inputFileName, "rb");
	if (NULL == pInputFile) {
		/*�ϊ��O�̃t�@�C�����J���̂Ɏ��s�����ꍇ�̏���*/
		printf("Failed to open %s\n", inputFileName);
		exit(1);
	}

	pOutputFile = fopen(outputFileName, "wb");
	if (NULL == pOutputFile) {
		/*�ϊ���̃t�@�C�����J���̂Ɏ��s�����ꍇ�̏���*/
		printf("Failed to open %s\n", outputFileName);
		exit(1);
	}

	/*24bit��Bitmap�t�@�C����8bit�ɕύX*/
	convertBitmap24bitTo8bit(pInputFile, pOutputFile);

	fclose(pInputFile);
	fclose(pOutputFile);

	printf("\n\nConversion succeeded.\n\nConverted file name is %s\n", outputFileName);

	/*�֐�strJoin����malloc���g�p���Ă���̂ŁA�������J��*/
	free(outputFileName);

	/*���������[�N���o*/
	_CrtDumpMemoryLeaks();

	return 0;
}
