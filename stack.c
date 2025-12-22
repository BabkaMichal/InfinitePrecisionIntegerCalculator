#include <stdio.h>
#include "bigint.h"

/* bigint stack init */
void bigint_stack_init(Stack* s) {
	s->top = -1;
}
/* pushes bigint onto stack */
int bigint_stack_push(Stack* s, BigInt* val) {
    if (s->top >= MAX_STACK - 1) {
        printf("Error: Stack overflow\n");
        return 0;
    }
    s->items[++(s->top)] = val;
    return 1;
}
/* returns and remove the bigint on top of stack */
BigInt* bigint_stack_pop(Stack* s) {
    if (s->top < 0) {
        printf("Error: Stack underflow");
        return NULL;
    }
    return s->items[(s->top)--];
}
/*  returns the bigint on top of stack */
BigInt* bigint_stack_peek(Stack* s) {
    if (s->top < 0) {
        printf("Error: Stack underflow");
        return NULL;
    }
    return s->items[(s->top)];
}
/* checks if stack is empty */
int bigint_stack_empty(Stack* s) {
    return s->top == -1;
}
void bigint_clear_stack(Stack* s) {
    while (!bigint_stack_empty(s)) {
        BigInt* item = bigint_stack_pop(s);
        bigint_free(item);
    }
}

/* operator stack init */
void op_stack_init(OpStack* s) {
    s->top = -1;
}
/* pushes operator onto stack */
int op_stack_push(OpStack* s, char* op) {
    if (s->top < MAX_STACK - 1) {
        s->items[++(s->top)] = op;
        return 1;
    }
    else {
        printf("Error: Operator stack overflow\n");
        return 0;
    }
}
/* returns and remove the operator on top of stack */
char* op_stack_pop(OpStack* s) {
    if (s->top < 0) {
        printf("Error: Stack underflow");
        return NULL;
    }
    return s->items[(s->top)--];
}
/* returns the operator on top of stack */
char* op_stack_peek(OpStack* s) {
    if (s->top < 0) {
        printf("Error: Stack underflow");
        return NULL;
    }
    return s->items[(s->top)];
}
/* checks if stack is empty */
int op_stack_empty(OpStack* s) {
    return s->top == -1;
}