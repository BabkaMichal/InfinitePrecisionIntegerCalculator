#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigInt.h"

BigInt* evaluate_rpn(char** tokens, int count) {
	/* variables */
	Stack stack;
	int i;
	BigInt *a, *b, *res, *num, *final_res;
	char* tok;

	/* stack inicialization */
	bigint_stack_init(&stack);

	/* looping through token array */
	for (i = 0; i < count; i++) {
		/* current token */
		tok = tokens[i];

		/* addition */
		if (strcmp(tok, "+") == 0) {
			/* first b then a */
			b = bigint_stack_pop(&stack);
			a = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!a || !b) {
				printf("Error: Missing operands for '+'\n");

				/* cleanup */
				if (a) bigint_free(a);
				if (b) bigint_free(b);
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* result */
			res = bigint_add(a, b);

			/* cleanup */
			bigint_free(a); bigint_free(b);

			/* calculation failed */
			if (!res) {
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_stack_clear(&stack);
				return NULL;
			}
		}

		/* substraction */
		else if (strcmp(tok, "-") == 0) {
			/* first b then a */
			b = bigint_stack_pop(&stack);
			a = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!a || !b) {
				printf("Error: Missing operands for '-'\n");

				/* cleanup */
				if (a) bigint_free(a);
				if (b) bigint_free(b);
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* result */
			res = bigint_sub(a, b);

			/* cleanup */
			bigint_free(a); bigint_free(b);

			/* calculation failed */
			if (!res) {
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_stack_clear(&stack);
				return NULL;
			}
		}

		/* multiplication */
		else if (strcmp(tok, "*") == 0) {
			/* first b then a */
			b = bigint_stack_pop(&stack);
			a = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!a || !b) {
				printf("Error: Missing operands for '*'\n");

				/* cleanup */
				if (a) bigint_free(a);
				if (b) bigint_free(b);
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* result */
			res = bigint_mult(a, b);

			/* cleanup */
			bigint_free(a); bigint_free(b);

			/* calculation failed */
			if (!res) {
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_stack_clear(&stack);
				return NULL;
			}
		}

		/* division */
		else if (strcmp(tok, "/") == 0) {
			/* first b then a */
			b = bigint_stack_pop(&stack);
			a = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!a || !b) {
				printf("Error: Missing operands for '/'\n");

				/* cleanup */
				if (a) bigint_free(a);
				if (b) bigint_free(b);
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* result */
			res = bigint_div(a, b);

			/* cleanup */
			bigint_free(a); bigint_free(b);

			/* calculation failed */
			if (!res) {
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_stack_clear(&stack);
				return NULL;
			}
		}

		/* modulo */
		else if (strcmp(tok, "%") == 0) {
			/* first b then a */
			b = bigint_stack_pop(&stack);
			a = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!a || !b) {
				printf("Error: Missing operands for '%%'\n");

				/* cleanup */
				if (a) bigint_free(a);
				if (b) bigint_free(b);
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* result */
			res = bigint_mod(a, b);

			/* cleanup */
			bigint_free(a); bigint_free(b);

			/* calculation failed */
			if (!res) {
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_stack_clear(&stack);
				return NULL;
			}
		}

		/* power to */
		else if (strcmp(tok, "^") == 0) {
			/* first b then a */
			b = bigint_stack_pop(&stack);
			a = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!a || !b) {
				printf("Error: Missing operands for '^'\n");

				/* cleanup */
				if (a) bigint_free(a);
				if (b) bigint_free(b);
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* result */
			res = bigint_pow(a, b);

			/* cleanup */
			bigint_free(a); bigint_free(b);

			/* calculation failed */
			if (!res) {
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_stack_clear(&stack);
				return NULL;
			}
		}

		/* negate */
		else if (strcmp(tok, "NEG") == 0) {
			/* getting the bigint */
			res = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!res) {
				printf("Error: Missing operands for 'NEG'\n");

				/* cleanup */
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* result */
			bigint_negate(res);

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_stack_clear(&stack);
				return NULL;
			}
		}

		/* factorial */
		else if (strcmp(tok, "!") == 0) {
			/* getting the bigint */
			a = bigint_stack_pop(&stack);

			/* not enought operands on stack */
			if (!a) {
				/* cleanup */
				printf("Error: Missing operands for '!'\n");
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* result */
			res = bigint_fact(a);

			/* cleanup */
			bigint_free(a);

			/* calculation failed */
			if (!res) {
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* safety check for push */
			if (!bigint_stack_push(&stack, res)) {
				bigint_free(res);
				bigint_stack_clear(&stack);
				return NULL;
			}
		}

		/* must be a number */
		else {
			num = str_to_bigint(tok);
			/* bigint allocation failed */
			if (!num) {
				printf("Error: Unknown token '%s'\n", tok);
				bigint_stack_clear(&stack);
				return NULL;
			}

			/* stack overflow */
			if (!bigint_stack_push(&stack, num)) {
				bigint_free(num);
				bigint_stack_clear(&stack);
				return NULL;
			}
		}
	}

	/* there should remain only one number */
	final_res = bigint_stack_pop(&stack);

	/* if something remain -> error */
	if (!bigint_stack_empty(&stack)) {
		printf("Error: Invalid expression (too many operands left)\n");

		/* cleanup */
		bigint_stack_clear(&stack);
		bigint_free(final_res);
		return NULL;
	}

	/* nothing in final_res */
	if (!final_res) {
		printf("Error: Empty expression or stack underflow\n");
		bigint_stack_clear(&stack);
		return NULL;
	}

	/* cleanup */
	bigint_stack_clear(&stack);

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
	/* variables */
	char* dst;

	dst = malloc(strlen(src) + 1);
	if (dst) strcpy(dst, src);
	return dst;
}

/* helper function for cleaning queue */
void queue_clear(char** output_queue, int output_idx) {
	int i;
	for (i = 0; i < output_idx; i++) free(output_queue[i]);
	free(output_queue);
}

/* shunting yard that converts infix notation (3 + 4) to postfix notation (3 4 +) */
char** shunting_yard(char* input, int* out_count) {
	/* variables */
	int capacity, output_idx, expecting_number, len, prec_op, prec_top, found_left;
	char** output_queue;
	OpStack op_stack;
	char *p, *start, *num_str, *top;
	char op[4];

	/* preparing the queue */
	capacity = strlen(input) + 10;
	output_queue = (char**)malloc(capacity * sizeof(char*));

	/* safety */
	if (!output_queue) {
		printf("Error: Out of memory\n");
		return NULL;
	}

	/* index for queue */
	output_idx = 0;

	/* preparing the operator stack */
	op_stack_init(&op_stack);

	/* variable for unary operators 1 -> number, 0 -> operand */
	expecting_number = 1;

	/* copying the string */
	p = input;

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
			/* save start of number */
			start = p;

			/* finding end of hexadecimal number */
			if (*p == '0' && (*(p + 1) == 'x' || *(p + 1) == 'X')){
				p += 2;
				while (isxdigit(*p)) p++;
			}

			/* finding the end of binary number */
			else if (*p == '0' && (*(p + 1) == 'b' || *(p + 1) == 'B')) {
				p += 2;
				while (*p == '0' || *p == '1') p++;
			}

			/* finding the end of decimal number */
			else {
				while (isdigit(*p)) p++;
			}
			
			/* make string from number -> find length -> alocate memory -> save number -> add end character */
			len = p - start;
			num_str = (char*)malloc(len + 1);
			strncpy(num_str, start, len);
			num_str[len] = '\0';

			/* save to queue */
			output_queue[output_idx++] = num_str;

			expecting_number = 0;
			continue;
		}

		/* its operand or bracket */
		/* buffer for operand */
		op[0] = 0; op[1] = 0; op[2] = 0; op[3] = 0;

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

			/* two operators in row */
			else if (expecting_number && strcmp(op, "!") != 0) {
				printf("Syntax error!");

				/* stack cleanup */
				op_stack_clear(&op_stack);

				/* queue cleanup */
				queue_clear(output_queue, output_idx);

				return NULL;
			}

			/* if factorial -> put it in queue */
			if (strcmp(op, "!") == 0) {
				output_queue[output_idx++] = my_strdup(op);
			}
			else
			{
				/* operator logic */
				while (!op_stack_empty(&op_stack)) {
					/* look at the top operator */
					top = op_stack_peek(&op_stack);

					/* if start bracket we end */
					if (strcmp(top, "(") == 0) break;

					/* get priority of top and current operator */
					prec_op = get_precedence(op);
					prec_top = get_precedence(top);

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
			}

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
			found_left = 0;
			/* add the stack to queue until we find starting bracket */
			while (!op_stack_empty(&op_stack)) {
				/* current char */
				top = op_stack_pop(&op_stack);
				/* found ( */
				if (strcmp(top, "(") == 0) {
					free(top);
					found_left = 1;
					break;
				}

				/* push to queue */
				output_queue[output_idx++] = top;
			}

			/* didnt find left bracket */
			if (!found_left) {
				printf("Error: Mismatched brackets\n");

				/* stack cleanup */
				op_stack_clear(&op_stack);

				/* queue cleanup */
				queue_clear(output_queue, output_idx);
			}
			p++;

			/* after brackets we expect operator */
			expecting_number = 0;
		}

		/* unknown character */
		else {
			printf("Error: Unknown character '%c'\n", *p);

			/* stack cleanup */
			op_stack_clear(&op_stack);

			/* queue cleanup */
			queue_clear(output_queue, output_idx);

			return NULL;
		}
	}

	/* if we expect number at end -> syntax error */
	if (expecting_number)
	{
		printf("Syntax error!");

		/* stack cleanup */
		op_stack_clear(&op_stack);

		/* queue cleanup */
		queue_clear(output_queue, output_idx);

		return NULL;
	}

	/* cleaning out the rest of stack to queue */
	while (!op_stack_empty(&op_stack)) {
		top = op_stack_pop(&op_stack);

		/* mismatched brackets */
		if (strcmp(top, "(") == 0 || strcmp(top, ")") == 0) {
			printf("Error: Mismatched brackets\n");

			/* stack cleanup */
			op_stack_clear(&op_stack);

			/* queue cleanup */
			queue_clear(output_queue, output_idx);

			/* current char cleanup */
			free(top);

			return NULL;
		}
		else {
			output_queue[output_idx++] = top;
		}
	}

	/* cleanup */
	op_stack_clear(&op_stack);

	*out_count = output_idx;
	return output_queue;
}