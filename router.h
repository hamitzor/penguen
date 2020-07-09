#ifndef _PENGUEN_ROUTER_H_
#define _PENGUEN_ROUTER_H_

#include "http_message.h"

struct route_t;

typedef void (*handler_t)(struct req_t *req, struct res_t *res);

typedef struct route_t
{
    char *uri;
    handler_t handler;
    struct route_t *_next;
} route_t;

void router_add_route(route_t **root_route, char *uri, handler_t handler);
route_t *router_get_route(route_t *root_route, char *uri);
void router_clear(route_t **root_route);

#endif