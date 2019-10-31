#pragma once

#define WINDOWS_BITMAP_FILE_SIZE 40

#define NOT_COMPRESSION 0

#define BI_PLANES_VALUE 1

/*1��f������̃f�[�^�T�C�Y (bit)*/
#define BI_BIT_COUNT_VALUE 8

/*���k�`��(�����k)*/
#define NOT_COMPRESSION 0

/*�c,���������ꂼ��̉𑜓x*/
#define BI_X_PELS_PER_METER_VALUE 0
#define BI_Y_PELS_PER_METER_VALUE 0

/*8bit�`���̏ꍇ�̃p���b�g��*/
#define BI_CLR_USED_8BIT 256

/*�d�v�ȃp���b�h�̃C���f�b�N�X(0�͂��ׂďd�v�̈Ӗ�)*/
#define BI_CLR_IMPORTANT_VALUE 0


typedef struct bitmapInfoHeader {
	unsigned long biSize;
	unsigned long  biWidth;
	long  biHeight;
	unsigned short  biPlanes;
	unsigned short  biBitCount;
	unsigned long biCompression;
	unsigned long biSizeImage;
	unsigned long  biXPelsPerMeter;
	unsigned long  biYPelsPerMeter;
	unsigned long biClrUsed;
	unsigned long biClrImportant;
}bitmapInfoHeader;