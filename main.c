#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigInt.h"

/* TODO: makefile */
/* TODO: repair negative factorial */
/* TODO: add error for missing command and for syntax error -> 1*+2 */

int main(int argc, char* argv[]) {
    /* variables */
    char buffer[1024];
    OutputMode current_mode = OUT_DEC;

    /* defaulty input stream is CLI */
    FILE* input_stream = stdin;

    /* if argument we change input stream to file */
    if (argc > 1) {
        input_stream = fopen(argv[1], "r");
        if (!input_stream) {
            perror("Invalid input file");
            return EXIT_FAILURE;
        }
    }

    /* main loop */
    while (1) {

        /* end of file or error */
        if (!fgets(buffer, sizeof(buffer), input_stream)) {
            break;
        }

        /* remove \n */
        buffer[strcspn(buffer, "\n")] = 0;

        /* processing one line */
        if (!handle_line(buffer, &current_mode)) {
            /* user quit */
            break;
        }
    }

    /* if we open file -> close */
    if (argc > 1 && input_stream) {
        fclose(input_stream);
    }

    return EXIT_SUCCESS;
}

