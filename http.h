#ifndef _PENGUEN_HTTP_H_
#define _PENGUEN_HTTP_H_

#include "conf.h"

typedef struct
{
    int sockfd;
    pgn_conf_t *conf;
} pgn_http_server_t;

pgn_http_server_t *pgn_http_create_server(pgn_conf_t *conf);
int pgn_http_listen(pgn_http_server_t *server);
int pgn_http_free(pgn_http_server_t *server);

#endif