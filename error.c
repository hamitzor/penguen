#include <stdio.h>
#include "error.h"

pgn_error_t pgn_errorno = PENGUEN_NO_ERROR;

char *pgn_error_messages[pgn_error_len] = {
    "No errors.",
    "Bad request.",
};

void pgn_print_error()
{
    fprintf(stderr, "[PENGUEN]: %s", pgn_error_messages[pgn_errorno]);
}

void pgn_set_error(pgn_error_t no)
{
    pgn_errorno = no;
}