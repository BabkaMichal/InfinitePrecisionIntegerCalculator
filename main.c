#include <stdio.h>
#include <stdlib.h>
#include "bigInt.h"

/* TODO: shunting yard -> dont forget "(" & ")" */
/* TODO: aritmetic operations -> factorial(gg), powerTo */
/* TODO: printing results in bin,dec,hex */
/* TODO: out -> prints current result number representation -> bin,dec,hex*/
/* TODO: quit -> ends process with EXIT_SUCCCESS */
/* memory will be cleaned in shunting yard */


int main() {
	BigInt* a = str_to_bigint("100");
	BigInt* b = str_to_bigint("-3");

	BigInt* c = bigint_div(a, b);

	print_debug_hex(c);
	printf("%d",c->sign);

	bigint_free(a);
	bigint_free(b);
	bigint_free(c);

	return EXIT_SUCCESS;
}

