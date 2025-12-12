#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "bigInt.h"

#define INITIALCAP 20
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

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
void bigIntFree(BigInt* n) {
    /* sanity check */
    if (!n) return;

    /* freeing the limbs */
    if (n->limbs)
        free(n->limbs);

    /* freeing the struct */
    free(n);
}

/* helper functions */
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
            /* memory reallocation */
            /* calculating new capacity */
            size_t new_capacity = n->capacity + INITIALCAP;
            
            /* temporary pointer if realloc fails */
            limb_t* new_limbs = (limb_t*)realloc(n->limbs, new_capacity * sizeof(limb_t));

            /* safety check */
            if (new_limbs == NULL) {
                printf("Error: out of memory for realloc!\n");
                return;
            }
            /* realloc will not clean out the newly allocated memory -> we do it ourselves */
            size_t old_capacity = n->capacity;
            memset(new_limbs + old_capacity, 0, (new_capacity - old_capacity) * sizeof(limb_t));

            /* saving the new pointers to struct */
            n->limbs = new_limbs;
            n->capacity = new_capacity;
        }
        /* saving the carry to next limb */
        n->limbs[n->size] = (limb_t)carry;
        n->size++;
    }
}

/* addition with logic */
BigInt* bigInt_add(BigInt* a, BigInt* b) {
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
            res = core_sub(b, a);
            res->sign = b->sign;
            return res;
        }
    }
}

/* substraction with logic */
BigInt* bigInt_sub(BigInt* a, BigInt* b) {
    /* variables */
    BigInt* res;

    /* sanity check */
    if (!a || !b) return NULL;

    /* we make the sign opposite */
    b->sign *= -1;

    /* we do addition -> a-b = a + (-b)*/
    res = bigInt_add(a, b);

    /* we bring the sign back to original*/
    b->sign *= 1;

    return res;
}

/* basic function that subs a - b, no logic */
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
    /* removing zeroes from start of the array */
    while (res->size > 1 && res->limbs[res->size - 1] == 0) {
        res->size--;
    }

    return res;
}

/* basic addition that adds two numbers together in absolute value */
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

/* return which number is bigger in absolute value */
int core_compare(BigInt* a, BigInt* b) {
    /* variables */
    size_t i;

    /* sanity check */
    if (!a || !b) return NULL;

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

/* calculates 2 to the power of $power */
BigInt* bigint_pow2(size_t power) {
    /* how much limbs do we need */
    size_t needed_limbs = (power / 16) + 1;

    /* sanity check */
    if (!power || !isdigit(power)) return NULL;

    /* initializing bigInt + 5 for reserve */
    BigInt* n = bigint_init(needed_limbs + 5); 

    /* setting needed size */
    n->size = needed_limbs;

    /* setting just one bit */
    size_t limb_index = power / 16;
    size_t bit_index = power % 16;

    /* multiply binary number by 2 (bitshift to left) */
    n->limbs[limb_index] = ((limb_t)1 << bit_index);

    return n;
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