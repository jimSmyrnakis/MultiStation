#define _CRT_SECURE_NO_WARNINGS
#include "Test/inc.h"
int Erg1Ask1(void);
int Erg1Ask2(void);
int Erg1Ask3(void);
int main(void) {
	printf("Starting Tests...\n");

	printf("=================================Askisi1 :\n========================================");
	int res = 0;// Erg1Ask1();
	if (res != 0)
		return res;

	res = 0;//Erg1Ask2();
	if (res != 0)
		return res;

	res = Erg1Ask3();
	if (res != 0)
		return res;

	return 0;
}