#include<stdio.h>

#include "bitmapInfoHeader.h"
#include "calc.h"

/*8bit�`��windowsBitmap�t�@�C���́A�摜�f�[�^�܂ł̃I�t�Z�b�g�T�C�Y
(�t�@�C���w�b�_)14 + (���w�b�_)40 + (�J���[�p���b�h)256*4 */
#define BF_OFF_BITS_VALUE 1078

void writeBitmapFileHeader(FILE* file,bitmapInfoHeader* pBIH) {

	/*�t�@�C���`���̏�������*/
	fwrite("BM", strlen("BM"), 1, file);

	/*�摜�f�[�^���̃T�C�Y�̌v�Z*/
	unsigned long pictureDataSize = calcMultipleOf4(pBIH->biWidth) * pBIH->biHeight;

	/*�t�@�C���T�C�Y���v�Z���ď�������*/
	unsigned long bfSize = BF_OFF_BITS_VALUE + pictureDataSize;
	fwrite(&bfSize, sizeof(bfSize), 1, file);

	/*�\��̈�̏�������*/
	signed short bfReserved1 = 0;
	fwrite(&bfReserved1, sizeof(bfReserved1), 1, file);
	signed short bfReserved2 = 0;
	fwrite(&bfReserved2, sizeof(bfReserved2), 1, file);

	/*�t�@�C���擪����摜�f�[�^�܂ł̃I�t�Z�b�g�̏�������*/
	unsigned long bfOffBits = BF_OFF_BITS_VALUE;
	fwrite(&bfOffBits, sizeof(bfOffBits), 1, file);
}