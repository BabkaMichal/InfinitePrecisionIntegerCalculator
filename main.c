#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bigint.h"

/* Upgrades: Make single responsibility principle in each class -> would take a lot of time */

int main(int argc, char* argv[]) {
    /* variables */
    int len, expr_len, expr_capacity, user;
    char last;
    char* expression;
    OutputMode current_mode;
    FILE* input_stream;

    /* buffer for one line */
    char buffer[BUFFER];

    /* expression for multiple lines */
    expr_capacity = EXPRESSION;
    expression = (char*)malloc(EXPRESSION * sizeof(char));

    /* safety */
    if (!expression) {
        return EXIT_FAILURE;
    }

    /* setting up empty string */
    expression[0] = '\0';

    /* default output mode = decimal */
    current_mode = OUT_DEC;

    /* defaulty input stream is CLI */
    user = 1;
    input_stream = stdin;

    /* if argument we change input stream to file */
    if (argc > 1) {
        input_stream = fopen(argv[1], "r");
        user = 0;
        if (!input_stream) {
            printf("Invalid input file!\n");
            free(expression);
            return EXIT_FAILURE;
        }
    }
    
    /* main loop */
    while (1) {
       
        /* end of file or error */
        if (!fgets(buffer, sizeof(buffer), input_stream)) {
            break;
        }

        printf("> ");
        fflush(stdout);

        /* remove whitespaces from end */
        len = strlen(buffer);
        while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r' || isspace(buffer[len - 1]))) {
            buffer[len - 1] = '\0';
            len--;
        }

        /* empty line */
        if (strlen(buffer) == 0) {
            continue;
        }

        /* concating lines if multiple */
        expr_len = strlen(expression);
        if (expr_len > 0) {
            last = expression[expr_len - 1];

            if (last != 'x' && last != 'b' && last != 'X' && last != 'B') {
                if (!safe_strcat(&expression, &expr_capacity, " ")) break;
            }
        }
        if (!safe_strcat(&expression, &expr_capacity, buffer)) {
            break;
        }

        /* if not end of a expression -> another loop */
        if (is_incomplete(expression)) {
            continue;
        }

        /* if from file -> print expression */
        if (!user) {
            printf("%s\n", expression);
        }

        /* handle one expression */
        if (!handle_line(expression, &current_mode)) {
                /* user quit */
                break;
        }

        /* cleaning out expression for another command */
        expression[0] = '\0';
    }

    /* if we open file -> close */
    if (argc > 1 && input_stream) {
        fclose(input_stream);
    }

    /* cleanup */
    free(expression);

    return EXIT_SUCCESS;
}

