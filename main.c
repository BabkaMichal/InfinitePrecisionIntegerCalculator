#include <stdio.h>
#include <stdlib.h>
#include "bigInt.h"

/* TODO: shunting yard -> dont forget "(" & ")" */
/* TODO: out -> prints current result number representation -> bin,dec,hex*/
/* TODO: quit -> ends process with EXIT_SUCCCESS */
/* memory will be cleaned in shunting yard */


int main() {
	BigInt* a = str_to_bigint("-100");
	BigInt* b = str_to_bigint("100");

	BigInt* c = bigint_fact(a);

	print_hex(c);
	print_bin(c);
	print_dec(c);

	printf("%d",c->sign);

	bigint_free(a);
	bigint_free(b);
	bigint_free(c);

	return EXIT_SUCCESS;
}

