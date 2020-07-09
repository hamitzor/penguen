#include <stdio.h>
#include "error.h"

penguen_error_t penguen_errorno = PENGUEN_NO_ERROR;

char *penguen_error_messages[penguen_error_len] = {
    "No errors.",
    "Bad request.",
    "Bad method.",
    "Bad http version.",
    "Bad header field."
};

void penguen_print_error()
{
    fprintf(stderr, "[PENGUEN]: %s", penguen_error_messages[penguen_errorno]);
}

void penguen_set_error(penguen_error_t no)
{
    penguen_errorno = no;
}