#include<stdio.h>
#include <stdlib.h>

/*
文字列結合にstrcat等を用いるとバッファオーバーフローの危険性があるため、自作した関数.
引数c1の後にc2を結合して、結合した文字列の先頭のポインタを返す.
引数c1,c2 : 結合する文字の先頭のポインタ
*/
char *strJoin(char *c1, char *c2) {

	/*連結する文字列の文字数('\0'を含まない)をそれぞれ取得*/
	unsigned int length1 = strlen(c1);
	unsigned int length2 = strlen(c2);

	/*連結後の文字列を格納する領域の確保*/
	char *ret = (char *)malloc(length1 + length2 + 1);

	/*文字列を連結*/
	snprintf(ret, length1 + length2 + 1, "%s%s", c1, c2);

	return ret;
}