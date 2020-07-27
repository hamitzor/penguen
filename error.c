#include <stdio.h>
#include <errno.h>
#include "error.h"

pgn_error_t pgn_errorno = PGN_NO_ERROR;

char *pgn_error_messages[pgn_error_len] = {
    "No errors.",
    "Bad request.",
    "Cannot create http server.",
    "Bad IP address."};

void pgn_print_error()
{
    fprintf(stderr, "[PENGUEN]: %s\n", pgn_error_messages[pgn_errorno]);
    if (errno != 0)
        perror("");
}

void pgn_set_error(pgn_error_t no)
{
    pgn_errorno = no;
}