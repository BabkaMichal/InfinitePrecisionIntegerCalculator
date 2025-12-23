#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "bigint.h"

#define INITIALCAP 20

/* router */
BigInt* str_to_bigint(const char* str) {
    /* sanity check */
    if (!str || str[0] == '\0') return NULL;

    /* if first char is zero and second is not end of string we think it is bin/hex */
    if (str[0] == '0' && str[1] != '\0')
    {
        /* bin */
        if (str[1] == 'B' || str[1] == 'b')
            return bin_to_bigint(str + 2);

        /* hex */
        if (str[1] == 'X' || str[1] == 'x')
            return hex_to_bigint(str + 2);
    }
    /* everything other is dec */
    return dec_to_bigint(str);
}

/* from string with decimal value to BigInt*/
BigInt* dec_to_bigint(const char* str) {
    /* variables */
    BigInt* n;
    int i;

    /* sanity check */
    if (!str) return NULL;

    /* initializing bigInt */
    n = bigint_init(INITIALCAP);

    /* if minus before number we take it as negative */
    if (str[0] == '-')
        n->sign = SIGN_NEG;

    /* looping through every digit and saving them to bigInt struct */
    for (i = 0; str[i] != '\0'; i++) {
        if (isdigit(str[i])) {
            bigint_mul_val_add_digit(n, str[i] - '0', 10);
        }
    }
    return n;
}

/* from string with binary value to BigInt */
BigInt* bin_to_bigint(const char* str) {
    /* variables */
    BigInt* raw;
    BigInt* limit;
    BigInt* result;
    size_t total_bits = 0;
    int i;

    /* sanity check */
    if (!str || str[0] == '\0') return NULL;

    /* loading raw value */
    raw = bigint_init(INITIALCAP);
    raw->sign = SIGN_POS;

    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == '0' || str[i] == '1') {
            bigint_mul_val_add_digit(raw, str[i] - '0', 2);
            total_bits++;
        }
    }
    /* checking if positive/negative */
    if (str[0] == '0') {
        /* starts with 0 -> positive */
        return raw;
    }
    else {
        /* starts with 1 -> negative */
        /* we need to calculate 2^amount of bits - raw*/

        /* calculation 2^amount of bits */
        limit = bigint_pow2(total_bits);

        /* calling the substraction function */
        result = core_sub(limit, raw);

        /* setting the sign to minus*/
        result->sign = SIGN_NEG;

        /* freeing temp memory */
        bigint_free(raw);
        bigint_free(limit);

        return result;
    }
}

/* from string with hexadecimal value to BigInt*/
BigInt* hex_to_bigint(const char* str) {
    /* variables */
    BigInt* raw;
    BigInt* limit;
    BigInt* result;
    size_t total_bits = 0;
    int i;
    int first_digit_val;

    /* sanity check */
    if (!str || str[0] == '\0') return NULL;

    /* loading raw value */
    raw = bigint_init(INITIALCAP);
    raw->sign = SIGN_POS;

    for (i = 0; str[i] != '\0'; i++) {
        /* converting char to number (0-15) */
        int val = hex_char_to_int(str[i]);
        /* calculating the limbs */
        bigint_mul_val_add_digit(raw, val, 16);

        /* because it hex, we need to add 4 bits for one char (A -> 1010) */
        total_bits += 4;
    }
    /* checking if positive/negative */
    first_digit_val = hex_char_to_int(str[0]);
    if (first_digit_val < 8) {
        /* (0-7) -> positive */
        return raw;
    }
    else {
        /* (8-15) -> negative */
        /* we need to calculate 2^amount of bits - raw*/

        /* calculation 2^amount of bits */
        limit = bigint_pow2(total_bits);

        /* calling the substraction function */
        result = core_sub(limit, raw);

        /* setting the sign to minus*/
        result->sign = SIGN_NEG;

        /* freeing temp memory */
        bigint_free(raw);
        bigint_free(limit);

        return result;
    }
}

/* helper class to convert hex char to int */
int hex_char_to_int(char c) {
    if (isdigit(c)) return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0; 
}

/* print current mode */
void print_current_mode(OutputMode mode) {
    switch (mode) {
    case OUT_DEC: printf("dec\n"); break;
    case OUT_BIN: printf("bin\n"); break;
    case OUT_HEX: printf("hex\n"); break;
    }
}

/* handles numeric input */
void process_input(char* input_line, OutputMode mode) {
    /* variables */
    int count = 0;

    /* shunting yard to transfer text to tokens */
    char** rpn_tokens = shunting_yard(input_line, &count);

    /* safety */
    if (!rpn_tokens) {
        printf("Error: parsing failed");
        return;
    }

    /* evaluate tokens */
    BigInt* result = evaluate_rpn(rpn_tokens, count);

    /* output */
    if (result) {
        /* checks which mode is used */
        switch (mode) {
        case OUT_DEC: print_dec(result); break;
        case OUT_BIN: print_bin(result); break;
        case OUT_HEX: print_hex(result); break;
        }
        /* cleanup */
        bigint_free(result);
    }

    /* cleanup */
    int i;
    for (i = 0; i < count; i++) {
        free(rpn_tokens[i]);
    }
    free(rpn_tokens);
}

/* checks if command or calculation */
int handle_line(char* buffer, OutputMode* current_mode) {
    /* ignore empty line */
    if (strlen(buffer) == 0) return 1;

    /* handles quit */
    if (strcmp(buffer, "quit") == 0) return 0;

    /* change of output mode */
    if (strcmp(buffer, "bin") == 0) {
        *current_mode = OUT_BIN;
        printf("bin\n");
        return 1;
    }
    if (strcmp(buffer, "dec") == 0) {
        *current_mode = OUT_DEC;
        printf("dec\n");
        return 1;
    }
    if (strcmp(buffer, "hex") == 0) {
        *current_mode = OUT_HEX;
        printf("hex\n");
        return 1;
    }

    /* prints out current output mode */
    if (strcmp(buffer, "out") == 0) {
        print_current_mode(*current_mode);
        return 1;
    }

    /* if not command calculate it */
    process_input(buffer, *current_mode);
    return 1;
}