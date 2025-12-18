#include <stdio.h>
#include <stdlib.h>
#include "bigInt.h"

/* TODO: shunting yard */
/* TODO: aritmetic operations -> divide */
/* memory will be cleaned in shunting yard */


int main() {
	BigInt* b = str_to_bigint("300");
	BigInt* a = str_to_bigint("300");

	BigInt* c = bigint_mult(a, b);

	print_debug_hex(c);
	printf("%d",c->sign);

	bigint_free(a);
	bigint_free(b);
	bigint_free(c);

	return EXIT_SUCCESS;
}

