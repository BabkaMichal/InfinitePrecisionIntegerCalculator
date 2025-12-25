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
#define BASE_STACK 256
#define INITIALCAP_BI 20
#define BUFFER 1024
#define EXPRESSION 2048

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
    BigInt** items;
    int top;
    int capacity;
} Stack;

/* operator stack */
typedef struct {
    char** items;
    int top;
    int capacity;
} OpStack;

/* enum for output modes */
typedef enum {
    OUT_DEC,
    OUT_HEX,
    OUT_BIN
} OutputMode;


/**
* @brief Initializes BigInt
* @param cap Initial capacity
* @return Pointer to initialized BigInt or NULL
*/
BigInt* bigint_init(size_t cap);

/**
* @brief Expands BigInt capacity
* @param n Pointer to BigInt that needs resizing
* @param new_size New capacity
* @return 0 for error, 1 for success
*/
int bigint_resize(BigInt* n, size_t new_size);

/**
* @brief Frees BigInt from memory.
* @param n Pointer to BigInt to be freed
*/
void bigint_free(BigInt* n);

/**
* @brief Makes a deep copy of BigInt
* @param src Pointer to source BigInt
* @return Pointer to copy of source BigInt
*/
BigInt* bigint_copy(BigInt* src);

/**
* @brief Frees whole queue
* @param output_queue Pointer to array of pointers to char
* @param output_idx Number of elements in queue
*/
void queue_clear(char** output_queue, int output_idx);

/**
* @brief Frees whole BigInt stack
* @param s Pointer to Stack struct
*/
void bigint_stack_clear(Stack* s);

/**
* @brief Frees whole operator stack
* @param s Pointer to OpStack struct
*/
void op_stack_clear(OpStack* s);

/**
* @brief Addition of two BigInts
* @param a Pointer to first BigInt
* @param b Pointer to second BigInt
* @return Pointer to new BigInt = a + b
*/
BigInt* bigint_add(BigInt* a, BigInt* b);

/**
* @brief Subtraction of two BigInts
* @param a Pointer to first BigInt
* @param b Pointer to second BigInt
* @return Pointer to new BigInt = a - b
*/
BigInt* bigint_sub(BigInt* a, BigInt* b);

/**
* @brief Multiplication of two BigInts
* @param a Pointer to first BigInt
* @param b Pointer to second BigInt
* @return Pointer to new BigInt = a * b
*/
BigInt* bigint_mult(BigInt* a, BigInt* b);

/**
* @brief Division of two BigInts
* @param a Pointer to first BigInt
* @param b Pointer to second BigInt
* @return Pointer to new BigInt = a / b
*/
BigInt* bigint_div(BigInt* a, BigInt* b);

/**
* @brief Modulo of two BigInts
* @param a Pointer to first BigInt
* @param b Pointer to second BigInt
* @return Pointer to new BigInt = a % b
*/
BigInt* bigint_mod(BigInt* a, BigInt* b);

/**
* @brief Calculates n raised to the power of exp
* @param n Pointer to base BigInt
* @param exp Pointer to exponent BigInt
* @return Pointer to new BigInt = n ^ exp
*/
BigInt* bigint_pow(BigInt* n, BigInt* exp);

/**
* @brief Factorial of BigInt
* @param n Pointer to BigInt
* @return Pointer to new BigInt = n!
*/
BigInt* bigint_fact(BigInt* n);

/**
* @brief Negates BigInt in place
* @param a Pointer to BigInt to be negated
*/
void bigint_negate(BigInt* a);

/**
* @brief Subtraction of two BigInts in absolute value (|a| - |b|)
* @note Assumes |a| >= |b|
* @param a Pointer to first BigInt
* @param b Pointer to second BigInt
* @return Pointer to new BigInt = (|a| - |b|)
*/
BigInt* core_sub(BigInt* a, BigInt* b);

/**
* @brief Addition of two BigInts in absolute value (|a| + |b|)
* @param a Pointer to first BigInt
* @param b Pointer to second BigInt
* @return Pointer to new BigInt = (|a| + |b|)
*/
BigInt* core_add(BigInt* a, BigInt* b);

/**
* @brief Multiplication of two BigInts in absolute value (|a| * |b|)
* @param a Pointer to first BigInt
* @param b Pointer to second BigInt
* @return Pointer to new BigInt = |a| * |b|
*/
BigInt* core_mult(BigInt* a, BigInt* b);

/**
* @brief Calculates integer division and remainder
* @param a Pointer to first BigInt - dividend
* @param b Pointer to second BigInt - divisor
* @param res_quotient Pointer to pointer for BigInt, used to store division results (a / b)
* @param res_remainder Pointer to pointer for BigInt, used to store modulo remainder (a % b)
* @return 1 on success, 0 on error
*/
int core_div_mod(BigInt* a, BigInt* b, BigInt** res_quotient, BigInt** res_remainder);

/**
* @brief Compares which number is bigger in absolute value
* @param a Pointer to first BigInt
* @param b Pointer to second BigInt
* @return 1 if |a| is greater, -1 if |b| is greater and 0 if they are the same value
*/
int core_compare(BigInt* a, BigInt* b);

/**
* @brief Calculates 2 ^ power
* @param power To what we power to
* @return Pointer to new BigInt containing 2 ^ power
*/
BigInt* bigint_pow2(size_t power);

/**
* @brief Multiplies BigInt by digit and adds val to it n = (n * digit) + val
* @param n Pointer to BigInt that we want to multiply and add value to
* @param digit Number which we multiply n by
* @param val Number which we add to n after multiplying
*/
void bigint_mul_val_add_digit(BigInt* n, int digit, int val);

/**
* @brief Checks if BigInt is zero
* @param n Pointer to BigInt which we want to check
* @return 1 if BigInt is zero, 0 otherwise
*/
int bigint_zero(BigInt* n);

/**
* @brief Trim leading zeros from BigInt (0001 -> 1)
* @param n Pointer to BigInt which we want to trim
*/
void bigint_trim(BigInt* n);

/**
* @brief Gets the value of bit in BigInt n on position bit_index
* @param n Pointer to BigInt which we want to check
* @param  bit_index Index of the bit we want to check
* @return 0 or 1 depending on BigInt
*/
int get_bit(BigInt* n, size_t bit_index);

/**
* @brief Sets the value of bit in BigInt n on position bit_index to 1
* @param n Pointer to BigInt which we want to set
* @param  bit_index Which bit we want to set
*/
void set_bit(BigInt* n, size_t bit_index);

/**
* @brief Bitshifts the entire BigInt to the left by one bit
* @param n Pointer to BigInt which we want to bitshift
*/
void bigint_shift_left(BigInt* n);

/**
* @brief Finds the first index of the most significant bit that is 1
* @param n Pointer to BigInt which we want to find highest bit in
* @return Position of highest bit or -1 if didnt find any
*/
long bigint_get_highest_bit(BigInt* n);

/**
* @brief Creates BigInt from string input
* @note Number without prefix are taken as decimal, 0b/0B as binary, 0x/0X as hexadecimal
* @note Assumes string contains numbers and hex characters
* @param str Input string containing numbers
* @return Pointer to BigInt containing the numeric value of the string
*/
BigInt* str_to_bigint(const char* str);

/**
* @brief Creates BigInt from string input
* @note Assumes string contains decimal input without prefix
* @param str Input string containing numbers
* @return Pointer to BigInt containing the numeric value of the string number
*/
BigInt* dec_to_bigint(const char* str);

/**
* @brief Creates BigInt from string input
* @note Assumes string contains hexadecimal input with 0x/0X prefix
* @param str Input string containing numbers
* @return Pointer to BigInt containing the numeric value of the string number
*/
BigInt* hex_to_bigint(const char* str);

/**
* @brief Creates BigInt from string input
* @note Assumes string contains binary input with 0b/0B prefix
* @param str Input string containing numbers
* @return Pointer to BigInt containing the numeric value of the string number
*/
BigInt* bin_to_bigint(const char* str);

/**
* @brief Prints the numeric value of BigInt in hexadecimal format
* @param n Pointer to BigInt to be printed
*/
void print_hex(BigInt* n);

/**
* @brief Prints the numeric value of BigInt in decimal format
* @param n Pointer to BigInt to be printed
*/
void print_dec(BigInt* n);

/**
* @brief Prints the numeric value of BigInt in binary format
* @param n Pointer to BigInt to be printed
*/
void print_bin(BigInt* n);

/**
* @brief Prints the current mode being used to print numbers
* @param mode OutputMode enum value to display
*/
void print_current_mode(OutputMode mode);

/**
* @brief Takes numeric expression and processes it
* @note Assumes input_line is expression
* @param input_line String containing the expression in infix form
* @param mode OutputMode enum specifying the format for printing results
*/
void process_input(char* input_line, OutputMode mode);
/**
* @brief Handles one line of expression, if command execute it, otherwise sent it into process_input
* @param buffer String to be checked
* @param current_mode Pointer for variable that stores current mode 
* @return 1 to continue, 0 to exit program
*/
int handle_line(char* buffer, OutputMode* current_mode);

/**
* @brief Safely appends a string to dynamic buffer, resizing if needed
* @param expt_ptr Pointer to a dynamic string array
* @param capacity Pointer to the integer tracking current buffer capacity
* @param to_add String we want to add
* @return 1 if success, 0 if failed
*/
int safe_strcat(char** expr_ptr, int* capacity, const char* to_add);

/**
* @brief Gets the numeric value from hexadecimal character
* @param c Hexadecimal character which we want to find value for
* @return Numeric value (0-15), -1 if error
*/
int hex_char_to_int(char c);

/**
* @brief Checks if string contains numbers
* @param s String we want to check
* @return 1 if the string has no numbers, 0 if it contains numbers
*/
int is_word(char* s);

/**
* @brief Checks if expression looks like it will continue on next line
* @param buffer Strings we want to check
* @return 1 if incomplete (continues), 0 if complete
*/
int is_incomplete(char* buffer);

/**
* @brief Initializes stack for BigInts
* @param s pointer to stack structure
*/
void bigint_stack_init(Stack* s);

/**
* @brief Pushes a BigInt onto stack
* @param s Pointer to the stack
* @param val Pointer to BigInt to be inserted
* @return 1 if successful, 0 if error
*/
int bigint_stack_push(Stack* s, BigInt* val);

/**
* @brief Removes and returns top BigInt from stack
* @param s Pointer to stack
* @return Pointer to BigInt
*/
BigInt* bigint_stack_pop(Stack* s);

/**
* @brief Returns top BigInt from stack
* @param s Pointer to the stack
* @return Pointer to BigInt
*/
BigInt* bigint_stack_peek(Stack* s);

/**
* @brief Checks if the stack is empty
* @param s Pointer to the stack
* @return 1 if the stack is empty, 0 otherwise
*/
int bigint_stack_empty(Stack* s);

/**
* @brief Initializes stack for strings(operators)
* @param s Pointer to OpStack structure
*/
void op_stack_init(OpStack* s);

/**
* @brief Pushes a string onto stack
* @param s Pointer to the stack
* @param op Pointer to string to be inserted
* @return 1 if successful, 0 if error
*/
int op_stack_push(OpStack* s, char* op);

/**
* @brief Removes and returns top string from stack
* @param s Pointer to stack
* @return Pointer to string
*/
char* op_stack_pop(OpStack* s);

/**
* @brief Returns top string from stack
* @param s Pointer to the stack
* @return Pointer to string
*/
char* op_stack_peek(OpStack* s);

/**
* @brief Checks if the stack is empty
* @param s Pointer to the stack
* @return 1 if the stack is empty, 0 otherwise
*/
int op_stack_empty(OpStack* s);

/**
* @brief Evaluate an array of tokens into finished BigInt result
* @param tokens Pointers to the array of tokens (strings)
* @param count Number of tokens in the array
* @return Result of the calculation from tokens
*/
BigInt* evaluate_rpn(char** tokens, int count);

/**
* @brief Returns the priority of input operator
* @param op The operator string we want to check
* @return Integer 0 to 5 representing priority of operator
*/
int get_precedence(char* op);

/**
* @brief Checks if the operator is right associative
* @param op Operator string we want to check
* @return 1 if right associative, 0 otherwise
*/
int is_right_associative(char* op);

/**
* @brief Makes a deep copy of the string
* @param src Source string
* @return Pointer to new string
*/
char* my_strdup(const char* src);

/**
* @brief Converts infix expression string into RPN
* @param input Input string containing mathematical expression
* @param out_count Pointer to an integer where the number of tokens will be stored
* @return Array of tokens in RPN, or NULL
*/
char** shunting_yard(char* input, int* out_count);

#endif