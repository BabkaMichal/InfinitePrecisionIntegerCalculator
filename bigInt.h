#ifndef BIGINT_H
#define BIGINT_H

#include <stdint.h>
#include <stddef.h>

/* constants */
#define SIGN_POS 1
#define SIGN_NEG -1
#define BASE 65536
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define CMP_ERR -2
#define MAX_STACK 256
#define INITIALCAP 20
#define BUFFER 1024
#define EXPRESSION 4096

/* typedef */
typedef unsigned short limb_t;
typedef unsigned long calc_t;

/* bigint */
typedef struct {
    limb_t* limbs;
    size_t size;
    size_t capacity;
    int sign;
} BigInt;

/* bigint stack */
typedef struct {
    BigInt* items[MAX_STACK];
    int top;
} Stack;

/* operator stack */
typedef struct {
    char* items[MAX_STACK];
    int top;
} OpStack;

/* enum for output modes */
typedef enum {
    OUT_DEC,
    OUT_HEX,
    OUT_BIN
} OutputMode;


/* memory functions */
BigInt* bigint_init(size_t cap);
void bigint_free(BigInt* n);
BigInt* bigint_copy(BigInt* src);


/* aritmethics - logic */
BigInt* bigint_add(BigInt* a, BigInt* b);
BigInt* bigint_sub(BigInt* a, BigInt* b);
BigInt* bigint_mult(BigInt* a, BigInt* b);
BigInt* bigint_div(BigInt* a, BigInt* b);
BigInt* bigint_mod(BigInt* a, BigInt* b);
BigInt* bigint_pow(BigInt* n, BigInt* exp);
BigInt* bigint_fact(BigInt* n);
void bigint_negate(BigInt* a);

/* arithmetics - core */
BigInt* core_sub(BigInt* a, BigInt* b);
BigInt* core_add(BigInt* a, BigInt* b);
BigInt* core_mult(BigInt* a, BigInt* b);
int core_div_mod(BigInt* a, BigInt* b, BigInt** res_quotient, BigInt** res_remainder);
int core_compare(BigInt* a, BigInt* b);

/* arithmetics - helper functions */
BigInt* bigint_pow2(size_t power);
void bigint_mul_val_add_digit(BigInt* n, int digit, int val);
int bigint_zero(BigInt* n);
void bigint_trim(BigInt* n);
int get_bit(BigInt* n, size_t bit_index);
void set_bit(BigInt* n, size_t bit_index);
void bigint_shift_left(BigInt* n);
long bigint_get_highest_bit(BigInt* n);

/* parsing and IO*/
BigInt* str_to_bigint(const char* str);
BigInt* dec_to_bigint(const char* str);
BigInt* hex_to_bigint(const char* str);
BigInt* bin_to_bigint(const char* str);
void print_hex(BigInt* n);
void print_dec(BigInt* n);
void print_bin(BigInt* n);
void print_current_mode(OutputMode mode);
void process_input(char* input_line, OutputMode mode);
int handle_line(char* buffer, OutputMode* current_mode);

/* helper IO functions */
int hex_char_to_int(char c);
int is_word(char* s);
int is_incomplete(char* buffer);

/* bigint stack */
void bigint_stack_init(Stack* s);
int bigint_stack_push(Stack* s, BigInt* val);
BigInt* bigint_stack_pop(Stack* s);
BigInt* bigint_stack_peek(Stack* s);
int bigint_stack_empty(Stack* s);
void bigint_clear_stack(Stack* s);

/* operator stack */
void op_stack_init(OpStack* s);
int op_stack_push(OpStack* s, char* op);
char* op_stack_pop(OpStack* s);
char* op_stack_peek(OpStack* s);
int op_stack_empty(OpStack* s);

/* evaluator and shunting yard */
BigInt* evaluate_rpn(char** tokens, int count);
int get_precedence(char* op);
int is_right_associative(char* op);
char* my_strdup(const char* src);
char** shunting_yard(char* input, int* out_count);

#endif