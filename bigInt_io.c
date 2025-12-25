#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "bigint.h"

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

    /* 0 -> positive */
    if (str[0] == '0') {
        return raw;
    }
    /* 1 -> negative */
    else {
        /* we need to calculate 2^amount of bits - raw*/
        /* calculating 2^amount of bits */
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
    int first_digit_val, i;

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

    /* getting current value of char */
    first_digit_val = hex_char_to_int(str[0]);

    /* positive */
    if (first_digit_val < 8) {
        return raw;
    }
    /* negative */
    else {
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

/* helper class to convert hex char to numeric value */
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
    int count = 0, i;

    /* shunting yard to transfer text to tokens */
    char** rpn_tokens = shunting_yard(input_line, &count);

    /* safety */
    if (!rpn_tokens) return;

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

    /* has no digits and wasnt picked up by other commands -> invalid */
    if (is_word(buffer)) {
        printf("Invalid command \"%s\"!\n", buffer);
        return 1;
    }

    /* if not command calculate it */
    process_input(buffer, *current_mode);
    return 1;
}

/* helper class that checks if string contains numbers */
int is_word(char* s) {
    while (*s) {
        if (!isalpha(*s)) return 0;
        s++;
    }
    return 1;
}

/* checks if command should continue on another line */
int is_incomplete(char* buffer) {
    /* getting length of string */
    int len = strlen(buffer);

    /* ignoring spaces at end */
    while (len > 0 && isspace(buffer[len - 1])) len--;

    /* empty line */
    if (len == 0) return 0;

    /* last character */
    char last = buffer[len - 1];

    /* ends with operator? */
    if (strchr("+-*/%^", last) || last == '(') return 1;

    /* ends with prefix? */
    if (len >= 2 && buffer[len - 2] == '0' && (last == 'x' || last == 'b')) {
        return 1;
    }

    /* if not it ended */
    return 0;
}

/* print bigint in hexadecimal */
void print_hex(BigInt* n) {
    /* sanity checks */
    if (!n) return;
    if (n->size == 0 || bigint_zero(n)) {
        printf("0x0\n");
        return;
    }

    /* variables */
    int i;

    /* sign and prefix */
    if (n->sign == SIGN_NEG) printf("-");
    printf("0x");

    /* biggest limb without padding */
    printf("%x", n->limbs[n->size - 1]);

    /* printing every limb */
    for (i = (int)n->size - 2; i >= 0; i--) {
        printf("%04x", n->limbs[i]);

    }
    printf("\n");
}

/* print bigint in decimal */
void print_dec(BigInt* n) {
    /* sanity checks */
    if (!n) return;
    if (n->size == 0 || bigint_zero(n)) {
        printf("0\n");
        return;
    }

    /* sign */
    if (n->sign == SIGN_NEG) printf("-");

    /* variables */
    /* base 10^9 -> 9 digits */
    BigInt* billion = str_to_bigint("1000000000");
    BigInt* temp = bigint_copy(n);
    int count = 0, i;

    /* buffer for result */
    /* capacity -> (n->size * 5) estimating the number of digits
    /9 -> transfering into chunks -> one chunk = 9 digits
    +10 -> safety */
    int capacity = (n->size * 5) / 9 + 10;
    calc_t* chunks = (calc_t*)malloc(capacity * sizeof(calc_t));

    /* safety */
    if (!chunks) {
        bigint_free(billion);
        bigint_free(temp);
        return;
    }

    /* main loop */
    while (!bigint_zero(temp)) {
        /* for saving division results */
        BigInt* quotient = NULL;
        BigInt* remainder = NULL;

        /* divide temp by billion -> remainder to buffer, quotient to another loop */
        if (!core_div_mod(temp, billion, &quotient, &remainder)) {
            /* failed */
            break;
        }

        /* put the result into calc_t(ulong) */
        calc_t rem_val = 0;
        if (remainder->size > 0) rem_val |= remainder->limbs[0];
        if (remainder->size > 1) rem_val |= ((calc_t)remainder->limbs[1] << 16);

        chunks[count++] = rem_val;

        /* move to the next step */
        bigint_free(temp);
        bigint_free(remainder);
        temp = quotient;
    }

    /* printing the result backwards */
    if (count > 0) {
        /* without 0 at start */
        printf("%lu", chunks[count - 1]);

        /* rest with padding */
        for (i = count - 2; i >= 0; i--) {
            printf("%09lu", chunks[i]);
        }
    }

    printf("\n");

    /* cleanup */
    free(chunks);
    bigint_free(billion);
    bigint_free(temp);
}

/* print bigint in binary */
void print_bin(BigInt* n) {
    /* sanity checks */
    if (!n) return;
    if (n->size == 0 || bigint_zero(n)) {
        printf("0b0\n");
        return;
    }

    /* variables */
    int i, j, bit, skipping = 1;
    int bits_per_limb = sizeof(limb_t) * 8;

    /* sign and prefix */
    if (n->sign == SIGN_NEG) printf("-");
    printf("0b");

    /* looping limbs from back */
    for (i = (int)n->size - 1; i >= 0; i--) {
        /* current limb */
        limb_t val = n->limbs[i];

        /* bits in limb from back */
        for (j = bits_per_limb - 1; j >= 0; j--) {
            /* extract the bit */
            bit = (val >> j) & 1;

            /* defaultly true -> when we find first 1 we stop */
            if (skipping) {
                if (bit == 0) {
                    continue;
                }
                else {
                    skipping = 0;
                }
            }

            /* print bit */
            printf("%d", bit);
        }
    }
    printf("\n");
}