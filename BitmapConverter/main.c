#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include"strJoin.h"
#include"calc.h"


/*24bit�`������8bit�`���ɕϊ������t�@�C���̖��O�̑O�ɕt���镶����*/
#define ADD_FILE_NAME "convert_"

#define BITMAP_FILEHEADER_REGION_SIZE 14
#define BF_TYPE_REGION_SIZE 2
#define FILE_TYPE_REGION_SIZE 2
#define BF_SIZ_REGION_SIZE 4
#define BF_RESERVED1_REGION_SIZE 2
#define BF_RESERVED2_REGION_SIZE 2
#define BF_OFF_BITS_REGION_SIZE 4
/*8bit�`���̃I�t�Z�b�g�̃T�C�Y
(�t�@�C���w�b�_)14 + (���w�b�_)40 + (�J���[�p���b�h)256*4 */
#define BF_OFF_BITS_VALUE 1078

#define BI_SIZE_REGION_SIZE 4
#define WINDOWS_BITMAP_FILE_SIZE 40

#define BI_WIDTH_REGION_SIZE 4
#define BI_HEIGHT_REGION_SIZE 4

#define BI_PLANES_REGION_SIZE 2
#define BI_PLANES_VALUE 1

#define BI_BIT_COUNT_REGION_SIZE 2
#define BI_BIT_COUNT_VALUE 8

#define BI_COMPRESSION_REGION_SIZE 4
#define NOT_COMPRESSION 0

#define BI_SIZE_IMAGE_REGION_SIZE 4

#define BI_PELS_PER_METER_REGION_SIZE 4
#define BI_X_PELS_PER_METER_VALUE 0
#define BI_Y_PELS_PER_METER_VALUE 0

#define BI_CLR_USED_REGION_SIZE 4
/*8bit�`���̏ꍇ�̃p���b�g��*/
#define BI_CLR_USED_8BIT 256

#define BI_CLR_IMPORTANT_REGION_SIZE 4
/*�d�v�ȃp���b�h�̃C���f�b�N�X(0�͂��ׂďd�v�̈Ӗ�)*/
#define BI_CLR_IMPORTANT_VALUE 0

int main(void) {

	FILE *inputFile = NULL;
	FILE *outputFile = NULL;

	/*�ϊ�����t�@�C���̖��O*/
	char inputFileName[] = "24sample.bmp";

	/*�ϊ���̃t�@�C����*/
	char *outputFileName = strJoin(ADD_FILE_NAME, inputFileName);

	int inputFileErr = fopen_s(&inputFile, inputFileName, "r");
	int outputFileErr = fopen_s(&outputFile, outputFileName, "w");

	/*�֐�strJoin����malloc���g�p���Ă���̂ŁA�������J��*/
	free(outputFileName);

	if (inputFileErr == 0 || outputFileErr == 0) {
		/*�t�@�C���̎擾�ɐ��������ۂ̏���*/

		/*�t�@�C���w�b�_�̏����i�[����̈���m��*/
		char bitmapFileHeader[BITMAP_FILEHEADER_REGION_SIZE];

		/*�t�@�C���w�b�_�̏����擾*/
		fread(&bitmapFileHeader, sizeof(char), BITMAP_FILEHEADER_REGION_SIZE, inputFile);

		/*�t�@�C���`��.
		2�o�C�g�i2�����j+�k���������̗̈���m�ۂ���.*/
		char bfType[BF_TYPE_REGION_SIZE + sizeof(char)];

		/*�t�@�C���`�����擾����*/
		snprintf(bfType, BF_TYPE_REGION_SIZE + sizeof(char), "%s", bitmapFileHeader);

		if (strcmp("BM", bfType) != 0) {
			/*�t�@�C���w�b�_�̃t�@�C���^�C�v��BM�łȂ������ۂ̏���*/
			printf("This file is not BMP file\n");
			exit(1);
		}



		/*���w�b�_�̃T�C�Y���擾*/
		unsigned char bitmapInfoHeaderSize = fgetc(inputFile);
		if (bitmapInfoHeaderSize != WINDOWS_BITMAP_FILE_SIZE) {
			/*windowsBitmap��40�o�C�g�Œ肾���AOS/2�̏ꍇ��12�o�C�g�炵���̂ŁAOS/2�������ꍇ�̏���*/
			/*TODO ���̑���Bitmap�t�@�C�����m�F�����ق�����������*/
			printf("This bitmap file is not windowsBitmap\n");
			exit(1);
		}

		/*�t�@�C���̈ʒu�w��q��1�o�C�g�߂��āA���w�b�_�̊J�n�ʒu��*/
		fseek(inputFile, -1, SEEK_CUR);

		/*���w�b�_�̏����i�[����̈�*/
		char *bitmapInfoHeader = (char *)malloc(bitmapInfoHeaderSize);
		if (bitmapInfoHeader == NULL) {
			/*�������̊����Ɏ��s�����ۂ̏���*/
			printf("Faild to allocate memory\n");
			exit(1);
		}

		/*���w�b�_�̏����擾*/
		fread(bitmapInfoHeader, sizeof(char), bitmapInfoHeaderSize, inputFile);

		/*�摜�̕��A�������擾*/
		unsigned long width = bitmapInfoHeader[4];
		unsigned long height = bitmapInfoHeader[8];

		/*1��f������̃f�[�^�T�C�Y*/
		short bitsPerPixel = bitmapInfoHeader[14];

		if (bitsPerPixel != 24) {
			/*�t�@�C����24bit�łȂ������ۂ̏���*/
			printf("This file is not 24bits");
			exit(1);
		}

		/*���k�`��*/
		int compression = bitmapInfoHeader[16];

		if (compression != NOT_COMPRESSION) {
			/*�t�@�C�������k����Ă���ۂ̏���*/
			printf("File is compressed");
			exit(1);
		}

		free(bitmapInfoHeader);



		/*�f�[�^��̉摜�̕�*/
		unsigned long widthMultipleOf4 = calcMultipleOf4(width);

		/*�摜�f�[�^�̃T�C�Y*/
		unsigned char pictureDataSize = (widthMultipleOf4 * height) * 3;

		/*�摜�f�[�^�̏����i�[����̈�*/
		unsigned char *pictureData = (unsigned char *)malloc(pictureDataSize);
		if (pictureData == NULL) {
			/*�������̊����Ɏ��s�����ۂ̏���*/
			printf("Faild to allocate memory\n");
			exit(1);
		}

		/*���w�b�_�̏����擾*/
		fread(pictureData, sizeof(char), pictureDataSize, inputFile);


		for (int i = 0; i < pictureDataSize; i++) {
			printf("%x,%x,%x\n", pictureData[i * 3], pictureData[i * 3 + 1], pictureData[i * 3 + 2]);
		}

		free(pictureData);

		/*�t�@�C���`������������*/
		fwrite("BM", sizeof(char), BF_TYPE_REGION_SIZE, outputFile);

		/*TODO �t�@�C���T�C�Y���v�Z���ď�������(���͓K����100����)*/
		unsigned long bfSize = 100;
		fwrite(&bfSize, sizeof(char), BF_SIZ_REGION_SIZE, outputFile);

		/*�\��̈����������*/
		signed short bfReserved1 = 0;
		fwrite(&bfReserved1, sizeof(char), BF_RESERVED1_REGION_SIZE, outputFile);
		signed short bfReserved2 = 0;
		fwrite(&bfReserved2, sizeof(char), BF_RESERVED2_REGION_SIZE, outputFile);

		/*�t�@�C���擪����摜�f�[�^�܂ł̃I�t�Z�b�g����������*/
		unsigned long bfOffBits = BF_OFF_BITS_VALUE;
		fwrite(&bfOffBits, sizeof(char), BF_OFF_BITS_REGION_SIZE, outputFile);

		/*���w�b�_�̃T�C�Y����������*/
		unsigned long biSize = WINDOWS_BITMAP_FILE_SIZE;
		fwrite(&biSize, sizeof(char), BI_SIZE_REGION_SIZE, outputFile);

		/*�摜�̕��ƍ�������������*/
		fwrite(&width, sizeof(char), BI_WIDTH_REGION_SIZE, outputFile);
		fwrite(&height, sizeof(char), BI_HEIGHT_REGION_SIZE, outputFile);

		/*�v���[��������������*/
		unsigned short biPlanes = BI_PLANES_VALUE;
		fwrite(&biPlanes, sizeof(char), BI_PLANES_REGION_SIZE, outputFile);

		/*1��f������̃f�[�^�T�C�Y����������*/
		unsigned short biBitCount = BI_BIT_COUNT_VALUE;
		fwrite(&biBitCount, sizeof(char), BI_BIT_COUNT_REGION_SIZE, outputFile);

		/*���k�`������������*/
		unsigned long biCompression = NOT_COMPRESSION;
		fwrite(&biCompression, sizeof(char), BI_COMPRESSION_REGION_SIZE, outputFile);

		/*�摜�f�[�^���̃T�C�Y����������*/
		unsigned long biSizeImage = widthMultipleOf4 * height;
		fwrite(&biSizeImage, sizeof(char), BI_SIZE_IMAGE_REGION_SIZE, outputFile);

		/*�𑜓x����������*/
		unsigned long biXPixPerMeter = BI_X_PELS_PER_METER_VALUE;
		unsigned long biYPixPerMeter = BI_Y_PELS_PER_METER_VALUE;
		fwrite(&biXPixPerMeter, sizeof(char), BI_PELS_PER_METER_REGION_SIZE, outputFile);
		fwrite(&biYPixPerMeter, sizeof(char), BI_PELS_PER_METER_REGION_SIZE, outputFile);

		/*�p���b�g���̏�������*/
		unsigned long biClrUsed = BI_CLR_USED_8BIT;
		fwrite(&biClrUsed, sizeof(char), BI_CLR_USED_REGION_SIZE, outputFile);

		/*�d�v�ȃp���b�g�̃C���f�b�N�X����������*/
		unsigned long biClrImportant = BI_CLR_IMPORTANT_VALUE;
		fwrite(&biClrImportant, sizeof(char), BI_CLR_IMPORTANT_REGION_SIZE, outputFile);

		/*TODO �J���[�p���b�h�̍쐬*/

	}
	else if (inputFileErr == ENOENT || outputFileErr == ENOENT) {
		/*�t�@�C�������݂��Ȃ������ۂ̏���*/
		printf("File is not exist\n");
		exit(1);
	}

	_fcloseall();


	printf("end");

	return 0;
}
