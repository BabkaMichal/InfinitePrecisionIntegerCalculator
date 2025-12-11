#ifndef bigInt
#define bigInt

typedef unsigned short limb_t;
typedef unsigned long calc_t;

typedef struct {
    limb_t* limbs;
    size_t size;
    size_t capacity;
} BigInt;

BigInt* bigint_init(size_t cap);

void bigint_mul_10_add_digit(BigInt* n, int digit);

BigInt* dec_to_bigint(const char* str);

void print_debug_hex(BigInt* n);

#endif