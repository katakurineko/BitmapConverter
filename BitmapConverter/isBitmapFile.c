#include<stdio.h>
#include <string.h>

#include "myBool.h"

#define BITMAP_FILEHEADER_REGION_SIZE 14

#define BF_TYPE_REGION_SIZE 2

char isBitmapFile(FILE* file) {

	/*�t�@�C���w�b�_�̏����i�[����̈�*/
	char bitmapFileHeader[BITMAP_FILEHEADER_REGION_SIZE] = { 0 };

	/*�t�@�C���w�b�_�̏����擾*/
	fread(&bitmapFileHeader, sizeof(bitmapFileHeader), 1, file);

	/*�t�@�C���`���̗̈�(2�o�C�g�i2�����j+1�o�C�g(�k��������))���m�ۂ���.*/
	char bfType[BF_TYPE_REGION_SIZE + sizeof(char)];

	/*�t�@�C���`�����擾����*/
	snprintf(bfType, BF_TYPE_REGION_SIZE + sizeof(char), "%s", bitmapFileHeader);

	if (0 != strcmp("BM", bfType)) {
		/*�t�@�C���w�b�_�̃t�@�C���^�C�v��BM�łȂ������ۂ̏���*/
		return FALSE;
	}

	return TRUE;
}