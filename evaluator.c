#include <ctype.h>
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

/* returns the priority of operator */
int get_precedence(char* op) {
	if (strcmp(op, "!") == 0 || strcmp(op, "^") == 0) return 4;
	if (strcmp(op, "NEG") == 0 || strcmp(op, "*") == 0 || strcmp(op, "/") == 0) return 3;
	if (strcmp(op, "%") == 0) return 2;
	if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
	return 0;
}

/* checks what associativity operator has */
int is_right_associative(char* op) {
	if (strcmp(op, "^") == 0 || strcmp(op, "NEG") == 0) return 1;
	return 0;
}

/* creates a copy of string */
char* my_strdup(const char* src) {
	char* dst = malloc(strlen(src) + 1);
	if (dst) strcpy(dst, src);
	return dst;
}

/* shunting yard that converts infix notation (3 + 4) to postfix notation (3 4 +) */
char** shunting_yard(char* input, int* out_count) {
	/* preparing the queue */
	int capacity = strlen(input) + 10;
	char** output_queue = (char**)malloc(capacity * sizeof(char*));

	/* safety */
	if (!output_queue) {
		printf("Error: Out of memory\n");
		return NULL;
	}

	/* index for queue */
	int output_idx = 0;

	/* preparing the operator stack */
	OpStack op_stack;
	op_stack_init(&op_stack);

	/* variable for unary operators 1 -> number, 0 -> operand */
	int expecting_number = 1;

	char* p = input;

	/* looping though the string */
	while (*p) {
		/* skipping spaces */
		if (isspace(*p))
		{
			p++;
			continue;
		}

		/* its a number */
		if (isdigit(*p)) {
			/* save start of number and find end of it */
			char* start = p;
			while (isdigit(*p)) p++;

			/* make string from number -> find length -> alocate memory -> save number -> add end character */
			int len = p - start;
			char* num_str = (char*)malloc(len + 1);
			strncpy(num_str, start, len);
			num_str[len] = '\0';

			/* save to queue */
			output_queue[output_idx++] = num_str;

			expecting_number = 0;
			continue;
		}

		/* its operand or bracket */
		/* buffer for operand */
		char op[4] = { 0 };

		/* finding operator */
		if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '%' || *p == '^' || *p == '!') {
			/* we save the operator */
			op[0] = *p;
			op[1] = '\0';
			p++;

			/* if we expect number and its - -> assume its negate  */
			if (strcmp(op, "-") == 0 && expecting_number) {
				strcpy(op, "NEG");
			}

			/* operator logic */
			while (!op_stack_empty(&op_stack)) {
				/* look at the top operator */
				char* top = op_stack_peek(&op_stack);

				/* if start bracket we end */
				if (strcmp(top, "(") == 0) break;

				/* get priority of top and current operator */
				int prec_op = get_precedence(op);
				int prec_top = get_precedence(top);

				/* if top stronger or equal & right associative we remove it from stack */
				if (prec_top > prec_op || (prec_top == prec_op && !is_right_associative(op))) {
					output_queue[output_idx++] = op_stack_pop(&op_stack);
				}
				else {
					break;
				}
			}
			/* push current operator */
			op_stack_push(&op_stack, my_strdup(op));

			/* we expect number, except factorial */
			if (strcmp(op, "!") == 0) expecting_number = 0;
			else expecting_number = 1;
		}

		/* starting bracket */
		else if (*p == '(') {
			op_stack_push(&op_stack, my_strdup("("));
			p++;
			expecting_number = 1;
		}

		/* ending bracket */
		else if (*p == ')') {
			/* add the stack to queue until we find starting bracket */
			while (!op_stack_empty(&op_stack)) {
				char* top = op_stack_pop(&op_stack);
				if (strcmp(top, "(") == 0) {
					free(top);
					break;
				}
				output_queue[output_idx++] = top;
			}
			p++;
			/* after brackets we expect operator */
			expecting_number = 0;
		}

		/* unknown character */
		else {
			printf("Error: Unknown character '%c'\n", *p);
			p++;
		}
	}

	/* cleaning out the rest of stack to queue */
	while (!op_stack_empty(&op_stack)) {
		char* top = op_stack_pop(&op_stack);

		/* mismatched brackets */
		if (strcmp(top, "(") == 0 || strcmp(top, ")") == 0) {
			printf("Error: Mismatched brackets\n");
			free(top);
		}
		else {
			output_queue[output_idx++] = top;
		}
	}

	*out_count = output_idx;
	return output_queue;
}