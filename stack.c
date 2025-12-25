#include <stdio.h>
#include <stdlib.h>
#include "bigint.h"

/* bigint stack init */
void bigint_stack_init(Stack* s) {
    s->capacity = BASE_STACK;
    s->top = -1;
    s->items = malloc(s->capacity * sizeof(BigInt*));
}

/* pushes bigint onto stack */
int bigint_stack_push(Stack* s, BigInt* val) {
    /* need bigger stack */
    if (s->top >= s->capacity - 1) {
        /* size + BASE_STACK */
        int new_cap = s->capacity + BASE_STACK;
        BigInt** new_items = realloc(s->items, new_cap * sizeof(BigInt*));

        /* safety */
        if (!new_items) return 0;

        /* copying items */
        s->items = new_items;
        s->capacity = new_cap;
    }

    /* pushing to stack */
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

/* clears bigint stack */
void bigint_stack_clear(Stack* s) {
    /* cleans bigints */
    while (!bigint_stack_empty(s)) {
        BigInt* item = bigint_stack_pop(s);
        bigint_free(item);
    }

    /* clean pointers */
    if (s->items) free(s->items);

    /* reset variables */
    s->top = -1;
    s->capacity = 0;
}

/* operator stack init */
void op_stack_init(OpStack* s) {
    s->capacity = BASE_STACK;
    s->top = -1;
    s->items = malloc(s->capacity * sizeof(char*));
}

/* pushes operator onto stack */
int op_stack_push(OpStack* s, char* op) {
    /* need bigger stack */
    if (s->top >= s->capacity - 1) {
        /* size + BASE_STACK */
        int new_cap = s->capacity + BASE_STACK;
        char** new_items = realloc(s->items, new_cap * sizeof(char*));

        /* safety */
        if (!new_items) return 0;

        /* copying items */
        s->items = new_items;
        s->capacity = new_cap;
    }

    /* pushing to stack */
    s->items[++(s->top)] = op;
    return 1;
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

/* clears operator stack */
void op_stack_clear(OpStack* s) {
    /* cleans chars */
    while (!op_stack_empty(s)) {
        char* item = op_stack_pop(s);
        free(item);
    }

    /* clean pointers */
    if (s->items) free(s->items);

    /* reset variables */
    s->top = -1;
    s->capacity = 0;
}