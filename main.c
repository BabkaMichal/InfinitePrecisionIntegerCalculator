#include <stdio.h>
#include <stdlib.h>
#include "bigInt.h"

/* TODO: shunting yard */
/* TODO: aritmetic operations -> multiply; divide */
/* memory will be cleaned in shunting yard */


int main() {
	BigInt* b = str_to_bigint("5");
	BigInt* a = str_to_bigint("-5");

	BigInt* c = bigInt_sub(a, b);

	print_debug_hex(c);
	printf("%d",c->sign);

	bigIntFree(a);
	bigIntFree(b);
	bigIntFree(c);

	return EXIT_SUCCESS;
}

