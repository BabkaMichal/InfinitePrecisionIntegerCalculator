#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigInt.h"

BigInt* evaluate_rpn(char** tokens, int count) {
	/* stack inicialization */
	Stack stack;
	bigint_stack_init(&stack);

	/* looping through token array */
	int i;
	for (i = 0; i < count; i++) {
		/* current token */
		char* tok = tokens[i];

		/* addition */
		if (strcmp(tok, "+") == 0) {
			/* first b then a */
			BigInt* b = bigint_stack_pop(&stack);
			BigInt* a = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!a || !b) {
				printf("Error: Missing operands for '+'\n");

				/* cleanup */
				if (a) bigint_free(a);
				if (b) bigint_free(b);
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* result */
			BigInt* res = bigint_add(a, b);

			/* cleanup */
			bigint_free(a); bigint_free(b);

			/* calculation failed */
			if (!res) {
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_clear_stack(&stack);
				return NULL;
			}
		}

		/* substraction */
		else if (strcmp(tok, "-") == 0) {
			/* first b then a */
			BigInt* b = bigint_stack_pop(&stack);
			BigInt* a = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!a || !b) {
				printf("Error: Missing operands for '-'\n");

				/* cleanup */
				if (a) bigint_free(a);
				if (b) bigint_free(b);
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* result */
			BigInt* res = bigint_sub(a, b);

			/* cleanup */
			bigint_free(a); bigint_free(b);

			/* calculation failed */
			if (!res) {
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_clear_stack(&stack);
				return NULL;
			}
		}

		/* multiplication */
		else if (strcmp(tok, "*") == 0) {
			/* first b then a */
			BigInt* b = bigint_stack_pop(&stack);
			BigInt* a = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!a || !b) {
				printf("Error: Missing operands for '*'\n");

				/* cleanup */
				if (a) bigint_free(a);
				if (b) bigint_free(b);
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* result */
			BigInt* res = bigint_mult(a, b);

			/* cleanup */
			bigint_free(a); bigint_free(b);

			/* calculation failed */
			if (!res) {
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_clear_stack(&stack);
				return NULL;
			}
		}

		/* division */
		else if (strcmp(tok, "/") == 0) {
			/* first b then a */
			BigInt* b = bigint_stack_pop(&stack);
			BigInt* a = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!a || !b) {
				printf("Error: Missing operands for '/'\n");

				/* cleanup */
				if (a) bigint_free(a);
				if (b) bigint_free(b);
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* result */
			BigInt* res = bigint_div(a, b);

			/* cleanup */
			bigint_free(a); bigint_free(b);

			/* calculation failed */
			if (!res) {
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_clear_stack(&stack);
				return NULL;
			}
		}

		/* modulo */
		else if (strcmp(tok, "%") == 0) {
			/* first b then a */
			BigInt* b = bigint_stack_pop(&stack);
			BigInt* a = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!a || !b) {
				printf("Error: Missing operands for '%%'\n");

				/* cleanup */
				if (a) bigint_free(a);
				if (b) bigint_free(b);
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* result */
			BigInt* res = bigint_mod(a, b);

			/* cleanup */
			bigint_free(a); bigint_free(b);

			/* calculation failed */
			if (!res) {
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_clear_stack(&stack);
				return NULL;
			}
		}

		/* power to */
		else if (strcmp(tok, "^") == 0) {
			/* first b then a */
			BigInt* b = bigint_stack_pop(&stack);
			BigInt* a = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!a || !b) {
				printf("Error: Missing operands for '^'\n");

				/* cleanup */
				if (a) bigint_free(a);
				if (b) bigint_free(b);
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* result */
			BigInt* res = bigint_pow(a, b);

			/* cleanup */
			bigint_free(a); bigint_free(b);

			/* calculation failed */
			if (!res) {
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_clear_stack(&stack);
				return NULL;
			}
		}

		/* negate */
		else if (strcmp(tok, "NEG") == 0) {
			/* only one bigint */
			BigInt* res = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!res) {
				printf("Error: Missing operands for 'NEG'\n");

				/* cleanup */
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* result */
			bigint_negate(res);

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_clear_stack(&stack);
				return NULL;
			}
		}

		/* factorial */
		else if (strcmp(tok, "!") == 0) {
			/* only one bigint */
			BigInt* a = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!a) {
				/* cleanup */
				printf("Error: Missing operands for '!'\n");
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* result */
			BigInt* res = bigint_fact(a);

			/* cleanup */
			bigint_free(a);

			/* calculation failed */
			if (!res) {
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_clear_stack(&stack);
				return NULL;
			}
		}

		/* must be a number */
		else {
			BigInt* num = str_to_bigint(tok);
			/* bigint allocation failed */
			if (!num) {
				printf("Error: Unknown token '%s'\n", tok);
				bigint_clear_stack(&stack);
				return NULL;
			}

			/* stack overflow */
			if (!bigint_stack_push(&stack, num)) {
				bigint_free(num);
				bigint_clear_stack(&stack);
				return NULL;
			}
		}
	}

	/* there should remain only one number */
	BigInt* final_res = bigint_stack_pop(&stack);

	/* if something remain -> error */
	if (!bigint_stack_empty(&stack)) {
		printf("Error: Invalid expression (too many operands left)\n");

		/* cleanup */
		bigint_clear_stack(&stack);
		bigint_free(final_res);
		return NULL;
	}

	/* nothing in final_res */
	if (!final_res) {
		printf("Error: Empty expression or stack underflow\n");
		bigint_clear_stack(&stack);
		return NULL;
	}

	return final_res;
}