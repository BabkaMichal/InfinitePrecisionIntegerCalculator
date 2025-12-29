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
    n = bigint_init(INITIALCAP_BI);

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
    BigInt *raw, *limit, *result;
    size_t total_bits = 0;
    int i;

    /* sanity check */
    if (!str || str[0] == '\0') return NULL;

    /* loading raw value */
    raw = bigint_init(INITIALCAP_BI);
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
    BigInt *raw, *limit, *result;
    size_t total_bits = 0;
    int first_digit_val, i;

    /* sanity check */
    if (!str || str[0] == '\0') return NULL;

    /* loading raw value */
    raw = bigint_init(INITIALCAP_BI);
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
    return -1; 
}

/* print current mode */
void print_current_mode(OutputMode mode) {
    switch (mode) {
    case OUT_DEC: printf("dec\n"); break;
    case OUT_BIN: printf("bin\n"); break;
    case OUT_HEX: printf("hex\n"); break;
    }
}

/* router that handles numeric input */
void process_input(char* input_line, OutputMode mode) {
    /* variables */
    int count = 0, i;
    char** rpn_tokens;
    BigInt* result;

    /* shunting yard to transfer text to tokens */
    rpn_tokens = shunting_yard(input_line, &count);

    /* safety */
    if (!rpn_tokens) return;

    /* evaluate tokens */
    result = evaluate_rpn(rpn_tokens, count);

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
        if (!isalpha(*s) && *s != '_') return 0;
        s++;
    }
    return 1;
}

/* checks if command should continue on another line */
int is_incomplete(char* buffer) {
    /* variables */
    int len;
    char last;

    /* getting length of string */
    len = strlen(buffer);

    /* ignoring spaces at end */
    while (len > 0 && isspace(buffer[len - 1])) len--;

    /* empty line */
    if (len == 0) return 0;

    /* last character */
    last = buffer[len - 1];

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
    /* variables */
    int i, lnz, k, skipping, shift, nibble, next_val;
    limb_t next_raw, next_trans, val;

    /* sanity checks */
    if (!n) return;
    if (n->size == 0 || bigint_zero(n)) {
        printf("0x0\n");
        return;
    }
    printf("0x");
    /* positive numbers */
    if (n->sign != SIGN_NEG) {
        /* if the most significant digit is 8-F we need to add 0 to start to mark it as positive  */
        if (n->limbs[n->size - 1] & 0x8000) {
            printf("0");
        }

        /* rest of the numbers in hexadecimal */
        printf("%x", n->limbs[n->size - 1]);
        for (i = (int)n->size - 2; i >= 0; i--) {
            printf("%04x", n->limbs[i]);
        }
    }

    /* negative numbers */
    else {
        /* find the lowest non zero limb */
        lnz = 0;
        for (k = 0; k < (int)n->size; k++) {
            if (n->limbs[k] != 0) {
                lnz = k;
                break;
            }
        }

        /* defaultly skipping zeros */
        skipping = 1;

        /* iterating through limbs from top */
        for (i = (int)n->size - 1; i >= 0; i--) {
            /* calculating the value */

            /* limb above LNZ -> negate, the carry is gone  */
            if (i > lnz) {
                /* we bit negate */
                val = ~n->limbs[i];
            }

            /* limb is lowest non zero limb -> we add the carry from calculations before */
            else if (i == lnz) {
                /* we negate and add 1 */
                val = (limb_t)(-n->limbs[i]);
            }

            /* limb below LNZ -> when we negate it overflows and puts carry into another limb */
            else {
                /* 0 stays 0 */
                val = 0;
            }

            /* iterate through 4 bits at a time */
            for (shift = 12; shift >= 0; shift -= 4) {
                /* getting one hexa character */
                nibble = (val >> shift) & 0xF;

                if (skipping) {
                    /* skipping leading F */
                    if (nibble == 0xF) {

                        /* no more digits */
                        next_val = -1;

                        /* finding the next 4 bits */
                        if (shift > 0) {
                            /* same limb */
                            next_val = (val >> (shift - 4)) & 0xF;
                        }
                        else if (i > 0) {
                            /* next limb -> need to calculate value*/
                            next_raw = n->limbs[i - 1];
                            if ((i - 1) > lnz) next_trans = ~next_raw;
                            else if ((i - 1) == lnz) next_trans = (limb_t)(-next_raw);
                            else next_trans = 0;

                            next_val = (next_trans >> 12) & 0xF;
                        }

                        /* last digit -> cannot skip it */
                        if (next_val == -1) {
                            skipping = 0;
                            printf("f");
                        }
                        else if (next_val >= 8) {
                            /* next digit is also F -> we can skip */
                            continue;
                        }
                        else {
                            /* next digit is 0-7 -> we need to keep f to says it is negative */
                            skipping = 0;
                            printf("f");
                        }
                    }
                    else {
                        /* start of the number */
                        skipping = 0;
                        printf("%x", nibble);
                    }
                }
                else {
                    /* print the rest */
                    printf("%x", nibble);
                }
            }
        }
    }
    printf("\n");
}

/* print bigint in decimal */
void print_dec(BigInt* n) {
    /* variables */
    BigInt *billion, *temp, * quotient, *remainder;
    int count, capacity, i;
    calc_t* chunks, rem_val;

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
    billion = str_to_bigint("1000000000");
    temp = bigint_copy(n);
    count = 0;

    /* buffer for result */
    /* capacity -> (n->size * 5) estimating the number of digits
    /9 -> transfering into chunks -> one chunk = 9 digits
    +10 -> safety */
    capacity = (n->size * 5) / 9 + 10;
    chunks = (calc_t*)malloc(capacity * sizeof(calc_t));

    /* safety */
    if (!chunks) {
        bigint_free(billion);
        bigint_free(temp);
        return;
    }

    /* main loop */
    while (!bigint_zero(temp)) {
        /* for saving division results */
        quotient = NULL;
        remainder = NULL;

        /* divide temp by billion -> remainder to buffer, quotient to another loop */
        if (!core_div_mod(temp, billion, &quotient, &remainder)) {
            /* failed */
            break;
        }

        /* put the result into calc_t(ulong) */
        rem_val = 0;
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
    /* variables */
    int skipping, bits_per_limb, j, bit;
    long msb_index, lsb_index, i;

    /* sanity checks */
    if (!n) return;
    if (n->size == 0 || bigint_zero(n)) {
        printf("0b0\n");
        return;
    }

    /* printing prefix */
    printf("0b");

    /* positive numbers */
    if (n->sign != SIGN_NEG) {
        /* must start with 0 */
        printf("0");

        /* skipping leading 0 */
        skipping = 1;
        bits_per_limb = sizeof(limb_t) * 8;

        /* looping through every limb */
        for (i = (int)n->size - 1; i >= 0; i--) {
            /* value of current limb */
            limb_t val = n->limbs[i];

            /* looping through every bit in limb */
            for (j = bits_per_limb - 1; j >= 0; j--) {
                /* getting current bit */
                bit = (val >> j) & 1;

                /* skipping leading zeros */
                if (skipping) {
                    if (bit == 0) continue;
                    skipping = 0;
                }

                printf("%d", bit);
            }
        }
    }

    /* negative numbers */
    else {
        /* find most significant bit */
        msb_index = bigint_get_highest_bit(n);
        lsb_index = -1;

        /* find least significant bit */
        for (i = 0; i <= msb_index; i++) {
            if (get_bit(n, i)) {
                lsb_index = i;
                break;
            }
        }

        /* if power of 2, we dont need to print 1 at start */
        if (msb_index != lsb_index) {
            printf("1");
        }

        /* bits to the left of LSB are inverted, rest stays the same */
        for (i = msb_index; i >= 0; i--) {
            bit = get_bit(n, i);

            if (i > lsb_index) {
                printf("%d", !bit);
            }
            else {
                printf("%d", bit);
            }
        }
    }

    printf("\n");
}

/* safelly strcat */
int safe_strcat(char** expr_ptr, int* capacity, const char* to_add) {
    /* variables */
    int curr_len, add_len, needed, new_cap;
    char* new_expr;

    /* getting current length */
    curr_len = strlen(*expr_ptr);
    add_len = strlen(to_add);

    /* needed length */
    needed = curr_len + add_len + 1;

    /* need bigger capacity */
    if (needed > *capacity) {
        /* current + EXPRESSION(1024) + safety(128) */
        new_cap = *capacity + EXPRESSION;
        if (new_cap < needed) new_cap = needed + 128;

        /* reallocation */
        new_expr = (char*)realloc(*expr_ptr, new_cap * sizeof(char));
        
        /* safety */
        if (!new_expr) {
            printf("Error: Out of memory!\n");
            return 0;
        }

        /* setting up pointers */
        *expr_ptr = new_expr;
        *capacity = new_cap;
    }

    /* we have space -> safely strcat */
    strcat(*expr_ptr, to_add);
    return 1;
}