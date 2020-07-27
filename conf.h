#ifndef _PENGUEN_CONF_H_
#define _PENGUEN_CONF_H_

#include "router.h"

#define PGN_ENABLE_GZIP 1

typedef struct
{
    char *addr;
    int port;
    int flags;
    char *base;
    char *static_dir;
    pgn_route_t *routes;
} pgn_conf_t;

void pgn_conf_free(pgn_conf_t *pgn_conf);

#endif