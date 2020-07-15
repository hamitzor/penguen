#ifndef _PENGUEN_ERROR_H_
#define _PENGUEN_ERROR_H_

typedef enum
{
    PENGUEN_NO_ERROR,
    PENGUEN_BAD_REQUEST,
    pgn_error_len
} pgn_error_t;

extern pgn_error_t pgn_errorno;

void pgn_print_error();

void pgn_set_error(pgn_error_t no);

#endif