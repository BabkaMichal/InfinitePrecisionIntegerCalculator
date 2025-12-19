#ifndef BIGINT_H
#define BIGINT_H

#include <stdint.h>
#include <stddef.h>

/* constants */
#define SIGN_POS 1
#define SIGN_NEG -1
#define BASE 65536

/* typedef */
typedef unsigned short limb_t;
typedef unsigned long calc_t;

/* struct */
typedef struct {
    limb_t* limbs;
    size_t size;
    size_t capacity;
    int sign;
} BigInt;

/* memory functions*/
BigInt* bigint_init(size_t cap);
void bigint_free(BigInt* n);


/* aritmethics - logic */
BigInt* bigint_add(BigInt* a, BigInt* b);
BigInt* bigint_sub(BigInt* a, BigInt* b);
BigInt* bigint_mult(BigInt* a, BigInt* b);
BigInt* bigint_div(BigInt* a, BigInt* b);
BigInt* bigint_mod(BigInt* a, BigInt* b);
void bigint_negate(BigInt* a);

/* arithmetics - core */
BigInt* core_sub(BigInt* a, BigInt* b);
BigInt* core_add(BigInt* a, BigInt* b);
BigInt* core_mult(BigInt* a, BigInt* b);
int core_div_mod(BigInt* a, BigInt* b, BigInt** res_quotient, BigInt** res_remainder);
int core_compare(BigInt* a, BigInt* b);

/* helper functions */
BigInt* bigint_pow2(size_t power);
void bigint_mul_val_add_digit(BigInt* n, int digit, int val);
int bigint_zero(BigInt* n);
void bigint_trim(BigInt* n);
int get_bit(BigInt* n, size_t bit_index);
void set_bit(BigInt* n, size_t bit_index);
void bigint_shift_left(BigInt* n);

/* parsing & IO*/
BigInt* str_to_bigint(const char* str);
BigInt* dec_to_bigint(const char* str);
BigInt* hex_to_bigint(const char* str);
BigInt* bin_to_bigint(const char* str);

/* helper IO functions */
int hex_char_to_int(char c);

/* temp */
void print_debug_hex(BigInt* n);

#endif