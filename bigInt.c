#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "bigInt.h"

#define INITIALCAP 20
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define CMP_ERR -2

/* initializes BigInt */
BigInt* bigint_init(size_t cap) {
    /* memory for bigInt structure*/
    BigInt* n = (BigInt*)malloc(sizeof(BigInt));

    /* sanity check */
    if (!n) return NULL;

    /* memory for the limbs */
    n->limbs = (limb_t*)calloc(cap, sizeof(limb_t));
    /* safety check */
    if (!n->limbs) {
        free(n);
        return NULL;
    }
    /* asigning values */
    n->size = 1;
    n->capacity = cap;
    n->sign = SIGN_POS;
    return n;
}

/* free BigInt from memory */
void bigint_free(BigInt* n) {
    /* sanity check */
    if (!n) return;

    /* freeing the limbs */
    if (n->limbs)
        free(n->limbs);

    /* freeing the struct */
    free(n);
}

/* expanding bigint */
int bigint_resize(BigInt* n, size_t new_size) {
    /* sanity check */
    if (!n) return 0;
    
    /* if we already have the size -> skip */
    if (n->capacity >= new_size) return 1;

    /* calculating new capacity */
    size_t new_capacity = new_size;

    /* temporary pointer if realloc fails */
    limb_t* new_limbs = (limb_t*)realloc(n->limbs, new_capacity * sizeof(limb_t));

    /* safety check */
    if (new_limbs == NULL) {
        fprintf(stderr, "\nFATAL ERROR: Out of memory! Allocation failed\n");
        fprintf(stderr, "Attempted to allocate %zu limbs.\n", new_capacity);

        /* if memory realocation failed, we end it */
        exit(EXIT_FAILURE);
    }

    /* realloc will not clean out the newly allocated memory -> we do it ourselves */
    size_t old_capacity = n->capacity;
    memset(new_limbs + old_capacity, 0, (new_capacity - old_capacity) * sizeof(limb_t));

    /* saving the new pointers to struct */
    n->limbs = new_limbs;
    n->capacity = new_capacity;

    /* success */
    return 1;
}

/* making new copy of bigint */
BigInt* bigint_copy(BigInt* src) {
    /* sanity check */
    if (!src) return NULL;

    /* keeping the capacity */
    BigInt* copy = bigint_init(src->capacity);

    /* safety check */
    if (!copy) return NULL;

    /* keeping everything */
    copy->size = src->size;
    copy->sign = src->sign;

    /* copying memory */
    memcpy(copy->limbs, src->limbs, src->size * sizeof(limb_t));

    return copy;
}

/* multiply the bigint by value and add digit */
void bigint_mul_val_add_digit(BigInt* n, int digit, int val) {
    /* variables */
    calc_t carry = digit;
    calc_t current;
    size_t i;

    /* sanity check */
    if (!n) return;
    
    for (i = 0; i < n->size; i++) {
        /* multiplying the value in limbs by value(dec ->10, bin -> 2 etc.) and adding carry to it */
        current = (calc_t)n->limbs[i] * val + carry;

        /* saving only the lower 16bits */
        n->limbs[i] = (limb_t)(current & 0xFFFF);

        /* shifting the upper 16bits to right and saving them to carry */
        carry = current >> 16;
    }

    /* if there is carry remaining, save to another limb */
    if (carry > 0) {
        if (n->size >= n->capacity) {
            /* memory realocation */
            if (!bigint_resize(n, n->capacity + INITIALCAP)) {
                return;
            }
        }
        /* saving the carry to next limb */
        n->limbs[n->size] = (limb_t)carry;
        n->size++;
    }
}

/* returns oposite number */
void bigint_negate(BigInt* n) {
    /* sanity check */
    if (!n) return;

    /* if zero -> we need to keep sign positive */
    if (bigint_zero(n))
    {
        n->sign = SIGN_POS;
        return;
    }

    /* return the same number in absolute value, just oposite sign*/
    n->sign *= -1;
}

/* addition with logic */
BigInt* bigint_add(BigInt* a, BigInt* b) {
    /* variables */
    BigInt* res;

    /* sanity check */
    if (!a || !b) return NULL;

    /* if a has the same sign as b, we add them */
    if (a->sign == b->sign)
    {
        res = core_add(a, b);
        res->sign = a->sign;
        return res;
    }
    /* different signs */
    else
    {
        /* finding the bigger number in absolute value */
        int cmp = core_compare(a, b);
        /* same number */
        if (cmp == 0)
        {
            /* returns 0 */
            res = bigint_init(1);
            res->sign = SIGN_POS;
            return res;
        }
        /* a is bigger */
        else if (cmp > 0) {
            /* a - b and sign from a */
            res = core_sub(a, b);
            res->sign = a->sign;
            return res;
        }
        /* b is bigger */
        else
        {
            /* b - a and sign from b */
            res = core_sub(b, a);
            res->sign = b->sign;
            return res;
        }
    }
}

/* substraction with sign logic */
BigInt* bigint_sub(BigInt* a, BigInt* b) {
    /* variables */
    BigInt* res;

    /* sanity check */
    if (!a || !b) return NULL;

    /* we make the sign opposite */
    b->sign *= -1;

    /* we do addition -> a-b = a + (-b)*/
    res = bigint_add(a, b);

    /* we bring the sign back to original*/
    b->sign *= 1;

    return res;
}

/* multiplication with sign logic */
BigInt* bigint_mult(BigInt* a, BigInt* b) {
    /* variables */
    BigInt* res;

    /* sanity check */
    if (!a || !b) return NULL;

    /* if one number 0 -> just return zero to save time*/
    if (bigint_zero(a) || bigint_zero(b))
    {
        return str_to_bigint("0");
    }

    /* setting the absolute value */
    res = core_mult(a, b);

    /* sign logic */
    res->sign = a->sign * b->sign;

    return res;
}

/* division with sing logic */
BigInt* bigint_div(BigInt* a, BigInt* b) {
    /* variables */
    BigInt* res;

    /* sanity check */
    if (!a || !b) return NULL;

    /* we only want the result -> if failed NULL*/
    if (!core_div_mod(a, b, &res, NULL)) return NULL;

    /* sign logic */
    res->sign = a->sign * b->sign;

    /* we need positive 0*/
    if (res->size == 1 && res->limbs[0] == 0) {
        res->sign = SIGN_POS;
    }

    return res;
}

/* modulo with sign logic */
BigInt* bigint_mod(BigInt* a, BigInt* b) {
    /* variables */
    BigInt* res;

    /* sanity check */
    if (!a || !b) return NULL;

    /* we only want the remainder -> if failed NULL*/
    if (!core_div_mod(a, b, NULL, &res)) return NULL;

    /* mod always has the same sign as the divident */
    res->sign = a->sign;

    /* we need positive 0*/
    if (res->size == 1 && res->limbs[0] == 0) {
        res->sign = SIGN_POS;
    }

    return res;
}

/* bigint powered to exponent */
BigInt* bigint_pow(BigInt* n, BigInt* exp) {
    /* sanity check */
    if (!n || !exp) return NULL;

    /* any number raised to the power of 0 is 1 */
    if (bigint_zero(exp)) return str_to_bigint("1");
    /* 0 raised to any power is 0 */
    if (bigint_zero(n)) return str_to_bigint("0");
    /* negative numbers returns fractions < 1 -> return 0 */
    if (exp->sign == SIGN_NEG) return str_to_bigint("0");

    /* result starts at 1 */
    BigInt* res = str_to_bigint("1");

    /* current base raised to power of 2 ^ i */
    BigInt* cur = bigint_copy(n);
    if (!cur) return NULL;
    cur->sign = SIGN_POS;

    /* finding the highest set bit to avoid iterations */
    long total_bits = bigint_get_highest_bit(exp);
    long i;

    for (i = 0; i <= total_bits; i++)
    {   /* if current bit == 1 -> we multiply current power with the result */
        if (get_bit(exp, i)) {
            BigInt* temp = bigint_mult(res, cur);
            bigint_free(res);
            res = temp;
        }

        /* avoid squaring in the final iteration */
        if (i < total_bits) {
            /* cur = cur ^ 2 */
            BigInt* next_cur = bigint_mult(cur, cur);
            bigint_free(cur);
            cur = next_cur;
        }
    }
    /* clean up */
    bigint_free(cur);

    /* sign logic */
    /* if negative we check if odd or even */
    if (n->sign == SIGN_NEG)
    {
        if (get_bit(exp, 0))
        {
            /* odd */
            res->sign = SIGN_NEG;
        }
        else
        {
            /* even */
            res->sign = SIGN_POS;
        }
    }
    else
    {
        res->sign = SIGN_POS;
    }

    return res;
}

BigInt* bigint_fact(BigInt* n) {
    /* sanity check */
    if (!n) return NULL;

    /* variables */
    BigInt* res;
    BigInt* i;
    BigInt* ONE;
    BigInt* temp;

    /* factorial of negative number doesnt exist */
    if (n->sign == SIGN_NEG) return NULL;

    /* edge case for 0! -> 1 */
    if (bigint_zero(n)) return str_to_bigint("1");
    
    /*  res -> result
        i -> iterator
        ONE -> constant to be added */
    res = str_to_bigint("1");
    i = str_to_bigint("2");
    ONE = str_to_bigint("1");

    /* safety check */
    if (!res || !i || !ONE) return NULL;

    /* if i <= n */
    while (core_compare(i, n) <= 0) {
        /* multiplying result with current iteration */
        temp = bigint_mult(res, i);

        /* safety check */
        if (!temp) {
            bigint_free(res); bigint_free(i); bigint_free(ONE);
            return NULL;
        }

        /* clean the old result and save the new one */
        bigint_free(res);
        res = temp;
        
        /* ONE++ */
        temp = bigint_add(i, ONE);

        /* safety check */
        if (!temp) {
            bigint_free(res); bigint_free(i); bigint_free(ONE);
            return NULL;
        }

        /* clean the old iterator and save the new one */
        bigint_free(i);
        i = temp;
    }

    /* cleaning the helper variables */
    bigint_free(i);
    bigint_free(ONE);

    /* factorial is always positive */
    res->sign = SIGN_POS;
    return res;
}

/* substraction in absolute value */
BigInt* core_sub(BigInt* a, BigInt* b) {
    /* setting up BigInt to store values */
    BigInt* res = bigint_init(a->capacity);
    res->size = a->size;

    /* sanity check */
    if (!a || !b) return NULL;

    /* variables */
    long diff;
    int borrow = 0;
    size_t i;


    for (i = 0; i < a->size; i++) {
        /* loading value from a */
        long val_a = a->limbs[i];

        /* loading value from b (if out of numbers -> 0) */
        long val_b = (i < b->size) ? b->limbs[i] : 0;

        /* a - b - number we borrowed from past calculation */
        diff = val_a - val_b - borrow;

        /* borrow check */
        if (diff < 0) {
            /* result is negative -> we need to borrow from higher tier */
            diff += BASE; /* 2^16 */
            borrow = 1;    /* sets the borrow for another round */
        }
        else {
            /* result is positive -> nothing */
            borrow = 0;
        }
        /* saving the positive result */
        res->limbs[i] = (limb_t)diff;
    }
    /* triming result */
    bigint_trim(res);

    return res;
}

/* addition in absolute value */
BigInt* core_add(BigInt* a, BigInt* b) {
    /* variables */
    size_t i;
    calc_t sum;
    calc_t carry = 0;

    /* sanity check */
    if (!a || !b) return NULL;

    /* initialize result BigInt -> can maximaly be larger than the max + 1 limb */
    size_t max_len = MAX(a->size, b->size);
    BigInt* res = bigint_init(max_len + 1);

    /* safety check */
    if (!res) return NULL;

    /* looping through the limbs */
    for (i = 0; i < max_len; i++)
    {
        /* load values from a,b */
        calc_t val_a = (i < a->size) ? a->limbs[i] : 0;
        calc_t val_b = (i < b->size) ? b->limbs[i] : 0;

        /* calulates sum of current limbs and carry from before*/
        sum = val_a + val_b + carry;

        /* stores the lower 16bits */
        res->limbs[i] = (limb_t)(sum & 0xFFFF);

        /* moves the upper 16 bits and saves them */
        carry = sum >> 16;
    }

    /* final carry */
    if (carry > 0)
    {
        res->limbs[max_len] = (limb_t)carry;
        res->size = max_len + 1;
    }
    else {
        res->size = max_len;
    }

    return res;
}

/* basic multiplication in absolute value */
BigInt* core_mult(BigInt* a, BigInt* b) {
    /* variables */
    size_t i, j;
    calc_t product;
    calc_t carry;

    /* sanity check */
    if (!a || !b) return NULL;

    /* initialize result BigInt */
    size_t max_len = a->size + b->size;
    BigInt* res = bigint_init(max_len);

    /* safety check */
    if (!res) return NULL;

    /* setting size to max so we can write everywhere */
    res->size = max_len;

    /* multiplying every limb from b with every limb from a */
    for (j = 0; j < b->size; j++)
    {
        /* new row -> reset carry */
        carry = 0;

        /* if digit in b is 0, skipping -> adds nothing */
        if (b->limbs[j] == 0) continue;

        for (i = 0; i < a->size; i++)
        {
            /* 
                res->limbs[i + j] -> what we stored from previous rows
                a[i] * b[j] -> current multiplication
                carry -> overflow
            */
            product = (calc_t)res->limbs[i + j] + ((calc_t)a->limbs[i] * (calc_t)b->limbs[j]) + carry;

            /* stores the lower 16bits */
            res->limbs[i + j] = (limb_t)(product & 0xFFFF);

            /* moves the upper 16 bits and saves them */
            carry = product >> 16;
        }
        /* storing the last carry to limbs */
        res->limbs[a->size + j] = (limb_t)carry;
    }
    /* trimming result */
    bigint_trim(res);

    return res;
}

/* division and modulo in absolute value */
int core_div_mod(BigInt* a, BigInt* b, BigInt** res_quotient, BigInt** res_remainder) {
    /* sanity check */
    if (!a || !b) return 0;

    /* division by 0 */
    if (b->size == 1 && b->limbs[0] == 0) {
        return 0;
    }

    /* variables */
    long i;
    long total_bits = (long)(a->size * 16);

    /* division result */
    BigInt* Q = bigint_init(a->size);
    /* making it so we can write everywhere */
    Q->size = a->size;

    /* remainder */
    BigInt* R = bigint_init(1);

    for (i = total_bits - 1;  i >= 0; i--)
    {
        /* bitshift R to left */
        bigint_shift_left(R);

        /* if current bit = 1 we add it to R */
        if (get_bit(a, i)) {
            R->limbs[0] |= 1;
        }

        /* removing zeroes from start of the array */
        bigint_trim(R);

        /* is remainder bigger than divisor */
        if (core_compare(R, b) >= 0) {
            
            /* we sub R = R - b */
            BigInt* temp_sub = core_sub(R, b);

            /* we set the new result */
            bigint_free(R);
            R = temp_sub;

            /* we put one into the result */
            set_bit(Q, i);
        }
    }
    /* trimming before returning */
    bigint_trim(Q);
    bigint_trim(R);

    /* preparing the result we need from the call */
    if (res_quotient)  *res_quotient = Q;
    else bigint_free(Q); 

    if (res_remainder) *res_remainder = R;
    else bigint_free(R); 

    return 1;
}

/* return which number is bigger in absolute value */
int core_compare(BigInt* a, BigInt* b) {
    /* variables */
    size_t i;

    /* sanity check */
    if (!a || !b) return CMP_ERR;

    /* if one has more limbs -> is longer */
    if (a->size > b->size) return 1;
    if (b->size > a->size) return -1;

    /* same number of limbs -> compare values in them */
    /* going from the biggest value limb -> from back */
    for (i = a->size ; i > 0; i--)
    {
        if (a->limbs[i - 1] > b->limbs[i - 1]) return 1;
        if (b->limbs[i - 1] > a->limbs[i - 1]) return -1;
    }
    /* they are the same */
    return 0;
}

/* calculates 2 to the power of n */
BigInt* bigint_pow2(size_t power) {
    /* how much limbs do we need */
    size_t needed_limbs = (power / 16) + 1;


    /* initializing bigInt + 5 for reserve */
    BigInt* n = bigint_init(needed_limbs + 5); 
    
    /* safety check */
    if (!n) return NULL;

    /* setting needed size */
    n->size = needed_limbs;

    /* setting just one bit */
    size_t limb_index = power / 16;
    size_t bit_index = power % 16;

    /* multiply binary number by 2 (bitshift to left) */
    n->limbs[limb_index] = ((limb_t)1 << bit_index);

    return n;
}

/* checking if zero */
int bigint_zero(BigInt* n) {
    return (n->size == 1 && n->limbs[0] == 0);
}

/* trims zeros from start: 0001 -> 1 */
void bigint_trim(BigInt* n) {
    while (n->size > 1 && n->limbs[n->size - 1] == 0) {
        n->size--;
    }
}

/* gets the bit value of the index */
int get_bit(BigInt* n, size_t bit_index) {
    /* sanity check */
    if (!n) return 0;
    
    /* calculating which limb is the bit in */
    size_t limb_idx = bit_index / 16;

    /* which exact bit it is */
    size_t bit_pos = bit_index % 16;

    /* safety memory check */
    if (limb_idx >= n->size) return 0;

    /* checks if 0 or 1 */
    return (n->limbs[limb_idx] >> bit_pos) & 1;
}

/* sets the bit to 1 on index */
void set_bit(BigInt* n, size_t bit_index) {
    /* sanity check */
    if (!n) return;

    /* calculating which limb is the bit in */
    size_t limb_idx = bit_index / 16;

    /* which exact bit it is */
    size_t bit_pos = bit_index % 16;

    /* we need more space */
    if (limb_idx >= n->size)
    {
        /* memory realocation */
        if (!bigint_resize(n, n->capacity + 16)) {
            return;
        }
    }
    /* sets the bit to 1 */
    n->limbs[limb_idx] |= (1 << bit_pos);
}

/* bit shifts the whole bigint to left */
void bigint_shift_left(BigInt* n) {
    /* sanity check */
    if (!n) return;

    /* variables */
    limb_t carry = 0;
    size_t i;

    /* loop through every limb */
    for (i = 0; i < n->size; i++) {
        /* save the upper bit */
        limb_t next_carry = n->limbs[i] >> 15;

        /* bit shift and add carry */
        n->limbs[i] = (n->limbs[i] << 1) | carry;

        /* assign carry for next loop */
        carry = next_carry;
    }

    if (carry)
    {
        /* if there is carry left we need to make one more limb */
        if (n->size == n->capacity) {
            /* safety check */
            if (!bigint_resize(n, n->capacity + INITIALCAP)) {
                return;
            }
        }
        /* we write the final carry */
        n->limbs[n->size] = 1;
        n->size++;
    }
}

/* finds the most significant bit */
long bigint_get_highest_bit(BigInt* n) {
    /* variables */
    long total_bits = (long)(n->size * 16);
    long i;

    /* looping from the back and finding first 1 */
    for (i = total_bits - 1; i >= 0; i--) {
        if (get_bit(n, i)) return i;
    }
    /* didnt find any */
    return 0;
}

/* temporary debug function*/
void print_debug_hex(BigInt* n) {
    int i;
    printf("Hex (16-bit chunks): ");
    for (i = 0; i < (int)n->size; i++) {
        /* %04x protoze tiskneme 16 bitu */
        printf("[%04x] ", n->limbs[i]);
    }
    printf("\n");
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

    int count = 0;

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

        /* put the result into calc_t -> ulong */
        calc_t rem_val = 0;
        if (remainder->size > 0) rem_val |= remainder->limbs[0];
        if (remainder->size > 1) rem_val |= ((calc_t)remainder->limbs[1] << 16);

        chunks[count++] = rem_val;

        /* move to next step */
        bigint_free(temp);
        bigint_free(remainder);
        temp = quotient;
    }
    /* printing the result backwards */
    if (count > 0) {
        /* without 0 at start */
        printf("%lu", chunks[count - 1]);

        int i;
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
    int i, j, bit;
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

            /* skiping first 0s */
            if (i == n->size - 1 && j == bits_per_limb - 1) {
                while (j >= 0 && !((val >> j) & 1)) {
                    j--;
                }
                /* the number is 0 */
                if (j < 0 && n->size == 1) {
                    printf("0");
                    break;
                }
                /* the limb was just 0 -> another limb */
                if (j < 0) continue;
            }

            /* extract the bit and print */
            bit = (val >> j) & 1;
            printf("%d", bit);
        }
    }
    printf("\n");
}