#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bigInt.h"

/* TODO: makefile */
/* opravit binarni vypis na praci s dvojkovym doplnkem -> bin a hex */
/* pri zpracovavani souboru vypisovat co zpracovavam */

int main(int argc, char* argv[]) {
    /* variables */
    char buffer[1024];
    char expression[4096] = "";
    OutputMode current_mode = OUT_DEC;

    /* defaulty input stream is CLI */
    FILE* input_stream = stdin;

    /* if argument we change input stream to file */
    if (argc > 1) {
        input_stream = fopen(argv[1], "r");
        if (!input_stream) {
            printf("Invalid input file!\n");
            return EXIT_FAILURE;
        }
    }
    
    /* main loop */
    while (1) {

        /* end of file or error */
        if (!fgets(buffer, sizeof(buffer), input_stream)) break;

        /* remove whitespaces from end */
        int len = strlen(buffer);
        while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r' || isspace(buffer[len - 1]))) {
            buffer[len - 1] = '\0';
            len--;
        }

        sanitize_input(buffer);

        /* empty line */
        if (strlen(buffer) == 0) continue;

        /* concating lines if multiple */
        int expr_len = strlen(expression);
        if (expr_len > 0) {
            char last = expression[expr_len - 1];

            if (last != 'x' && last != 'b' && last != 'X' && last != 'B') {
                strcat(expression, " ");
            }
        }
        strcat(expression, buffer);

        /* if not end of a expression -> another loop */
        if (is_incomplete(expression)) continue;

        /* processing one line */
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

    return EXIT_SUCCESS;
}

