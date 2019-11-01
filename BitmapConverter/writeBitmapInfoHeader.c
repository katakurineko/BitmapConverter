#include<stdio.h>

#include "bitmapInfoHeader.h"
#include "calc.h"

void writeBitmapInfoHeader(FILE* pFile, bitmapInfoHeader* pBIH) {
	/*���w�b�_�̃T�C�Y�̏�������*/
	unsigned long biSize = WINDOWS_BITMAP_FILE_SIZE;
	fwrite(&biSize, sizeof(biSize), 1, pFile);

	/*�摜�̕��ƍ�������������*/
	fwrite(&(pBIH->biWidth), sizeof(pBIH->biWidth), 1, pFile);
	fwrite(&(pBIH->biHeight), sizeof(pBIH->biHeight), 1, pFile);

	/*�v���[��������������*/
	unsigned short biPlanes = BI_PLANES_VALUE;
	fwrite(&biPlanes, sizeof(biPlanes), 1, pFile);

	/*1��f������̃f�[�^�T�C�Y����������*/
	unsigned short biBitCount = BI_BIT_COUNT_VALUE;
	fwrite(&biBitCount, sizeof(biBitCount), 1, pFile);

	/*���k�`������������*/
	unsigned long biCompression = NOT_COMPRESSION;
	fwrite(&biCompression, sizeof(biCompression), 1, pFile);

	/*�摜�f�[�^���̃T�C�Y�̌v�Z*/
	unsigned long pictureDataSize = calcMultipleOf4(pBIH->biWidth) * pBIH->biHeight;
	
	/*�摜�f�[�^���̃T�C�Y����������*/
	fwrite(&pictureDataSize, sizeof(pictureDataSize), 1, pFile);

	/*�𑜓x����������*/
	unsigned long biXPixPerMeter = BI_X_PELS_PER_METER_VALUE;
	unsigned long biYPixPerMeter = BI_Y_PELS_PER_METER_VALUE;
	fwrite(&biXPixPerMeter, sizeof(biXPixPerMeter), 1, pFile);
	fwrite(&biYPixPerMeter, sizeof(biYPixPerMeter), 1, pFile);

	/*�p���b�g���̏�������*/
	unsigned long biClrUsed = BI_CLR_USED_8BIT;
	fwrite(&biClrUsed, sizeof(biClrUsed), 1, pFile);

	/*�d�v�ȃp���b�g�̃C���f�b�N�X����������*/
	unsigned long biClrImportant = BI_CLR_IMPORTANT_VALUE;
	fwrite(&biClrImportant, sizeof(biClrImportant), 1, pFile);
}