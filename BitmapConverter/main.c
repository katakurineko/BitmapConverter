#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include"strJoin.h"
#include"calc.h"
#include"pixelDataRGB.h"


/*24bit�`������8bit�`���ɕϊ������t�@�C���̖��O�̑O�ɕt���镶����*/
#define ADD_FILE_NAME "convert_"

#define BITMAP_FILEHEADER_REGION_SIZE 14
#define BF_TYPE_REGION_SIZE 2
/*8bit�`���̃I�t�Z�b�g�̃T�C�Y
(�t�@�C���w�b�_)14 + (���w�b�_)40 + (�J���[�p���b�h)256*4 */
#define BF_OFF_BITS_VALUE 1078

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

	FILE *inputFile = NULL;

	/*�ϊ�����t�@�C���̖��O*/
	char inputFileName[] = "24sample.bmp";


	int inputFileErr = fopen_s(&inputFile, inputFileName, "rb");

	if (inputFileErr == 0) {
		/*�t�@�C���̎擾�ɐ��������ۂ̏���*/

		FILE *outputFile = NULL;

		/*�ϊ���̃t�@�C����*/
		char *outputFileName = strJoin(ADD_FILE_NAME, inputFileName);

		int outputFileErr = fopen_s(&outputFile, outputFileName, "wb");


		if (0 != outputFileErr) {
			printf("Unexpected error has occurred\n");
			exit(1);
		}

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

		/*�s�N�Z����*/
		unsigned long inputFileAllPixelNum = widthMultipleOf4 * height;

		/*�摜�f�[�^�̃T�C�Y*/
		unsigned long pictureDataSize = inputFileAllPixelNum * 3;

		/*�t�@�C���`������������*/
		fwrite(bfType, strlen(bfType), 1, outputFile);

		/*�t�@�C���T�C�Y���v�Z���ď�������*/
		unsigned long bfSize = BF_OFF_BITS_VALUE + inputFileAllPixelNum;
		fwrite(&bfSize, sizeof(bfSize), 1, outputFile);

		/*�\��̈����������*/
		signed short bfReserved1 = 0;
		fwrite(&bfReserved1, sizeof(bfReserved1), 1, outputFile);
		signed short bfReserved2 = 0;
		fwrite(&bfReserved2, sizeof(bfReserved2), 1, outputFile);

		/*�t�@�C���擪����摜�f�[�^�܂ł̃I�t�Z�b�g����������*/
		unsigned long bfOffBits = BF_OFF_BITS_VALUE;
		fwrite(&bfOffBits, sizeof(bfOffBits), 1, outputFile);

		/*���w�b�_�̃T�C�Y����������*/
		unsigned long biSize = WINDOWS_BITMAP_FILE_SIZE;
		fwrite(&biSize, sizeof(biSize), 1, outputFile);

		/*�摜�̕��ƍ�������������*/
		fwrite(&width, sizeof(width), 1, outputFile);
		fwrite(&height, sizeof(height), 1, outputFile);

		/*�v���[��������������*/
		unsigned short biPlanes = BI_PLANES_VALUE;
		fwrite(&biPlanes, sizeof(biPlanes), 1, outputFile);

		/*1��f������̃f�[�^�T�C�Y����������*/
		unsigned short biBitCount = BI_BIT_COUNT_VALUE;
		fwrite(&biBitCount, sizeof(biBitCount), 1, outputFile);

		/*���k�`������������*/
		unsigned long biCompression = NOT_COMPRESSION;
		fwrite(&biCompression, sizeof(biCompression), 1, outputFile);

		/*�摜�f�[�^���̃T�C�Y����������*/
		unsigned long biSizeImage = inputFileAllPixelNum;
		fwrite(&biSizeImage, sizeof(biSizeImage), 1, outputFile);

		/*�𑜓x����������*/
		unsigned long biXPixPerMeter = BI_X_PELS_PER_METER_VALUE;
		unsigned long biYPixPerMeter = BI_Y_PELS_PER_METER_VALUE;
		fwrite(&biXPixPerMeter, sizeof(biXPixPerMeter), 1, outputFile);
		fwrite(&biYPixPerMeter, sizeof(biYPixPerMeter), 1, outputFile);

		/*�p���b�g���̏�������*/
		unsigned long biClrUsed = BI_CLR_USED_8BIT;
		fwrite(&biClrUsed, sizeof(biClrUsed), 1, outputFile);

		/*�d�v�ȃp���b�g�̃C���f�b�N�X����������*/
		unsigned long biClrImportant = BI_CLR_IMPORTANT_VALUE;
		fwrite(&biClrImportant, sizeof(biClrImportant), 1, outputFile);

		/*�J���[�p���b�h�̍쐬*/
		unsigned char rgbColorPalette = 0;
		unsigned char colorPaletteReserved = 0;
		for (int i = 0; i < BI_CLR_USED_8BIT; i++) {
			for (int j = 0; j < 3; j++) {
				fwrite(&rgbColorPalette, sizeof(rgbColorPalette), 1, outputFile);
			}
			fwrite(&colorPaletteReserved, sizeof(colorPaletteReserved), 1, outputFile);
			rgbColorPalette++;
		}


		/*�摜�f�[�^�̏����i�[����̈�*/
		unsigned char *pictureData = (unsigned char *)malloc(pictureDataSize);

		if (pictureData == NULL) {
			/*�������̊����Ɏ��s�����ۂ̏���*/
			printf("Faild to allocate memory\n");
			exit(1);
		}

		/*�摜�f�[�^�̏����擾*/
		fread(pictureData, sizeof(char), pictureDataSize, inputFile);

		/*�ϊ��O�摜�̉摜�f�[�^�i�[�p�\����*/
		pixelDataRGB inputFilePixelData;

		/*�摜�f�[�^�̕ϊ�*/
		for (unsigned long i = 0; i < inputFileAllPixelNum; i++) {

			/*�v���O���X�\��*/
			printf("\r[%lu/%lu](%3.3f%%)",i, inputFileAllPixelNum,i*100/inputFileAllPixelNum);

			inputFilePixelData.green = pictureData[i * 3];
			inputFilePixelData.blue = pictureData[i * 3 + 1];
			inputFilePixelData.red = pictureData[i * 3 + 2];
			unsigned char convertedData = calcLuminance(&inputFilePixelData);
			fwrite(&convertedData, sizeof(convertedData), 1, outputFile);
		}

		free(pictureData);

		printf("Conversion succeeded.\n\nConverted file name is %s\n", outputFileName);

		/*�֐�strJoin����malloc���g�p���Ă���̂ŁA�������J��*/
		free(outputFileName);

	}
	else if (inputFileErr == ENOENT) {
		/*�t�@�C�������݂��Ȃ������ۂ̏���*/
		printf("File is not exist\n");
		exit(1);
	}

	_fcloseall();




	return 0;
}
