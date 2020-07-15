#ifndef _PENGUEN_ERROR_H_
#define _PENGUEN_ERROR_H_

typedef enum
{
    PGN_NO_ERROR,
    PGN_BAD_REQUEST,
    PGN_CANNOT_CREATE_SERVER,
    PGN_BAD_IP_ADDRESS,
    pgn_error_len
} pgn_error_t;

extern pgn_error_t pgn_errorno;

void pgn_print_error();

void pgn_set_error(pgn_error_t no);

#endif