

/*0を含む任意の正の整数の、次に現れる4の倍数を返す関数.*/
unsigned long calcMultipleOf4(unsigned long x) {
	return ((x - 1) / 4 + 1) * 4;
}