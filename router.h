#ifndef _PENGUEN_ROUTER_H_
#define _PENGUEN_ROUTER_H_

#include "http_message.h"

typedef void (*handler_t)(char *uri, pgn_res_t *res);

typedef struct pgn_route_t
{
    char *uri;
    char *file_path;
    handler_t handler;
    struct pgn_route_t *_next;
} pgn_route_t;

void pgn_router_add_route(pgn_route_t **root_route, char *uri, char *file_path, handler_t handler);
pgn_route_t *pgn_router_get_route(pgn_route_t *root_route, char *uri);
void pgn_router_free(pgn_route_t *root_route);

#endif
