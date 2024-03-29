#include "router.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void _create_new_route(pgn_route_t **route, char *uri, char *file_path, handler_t handler)
{
    *route = (pgn_route_t *)malloc(sizeof(pgn_route_t));
    (*route)->uri = uri;
    (*route)->file_path = file_path;
    (*route)->handler = handler;
    (*route)->_next = NULL;
}

void pgn_router_add_route(pgn_route_t **root_route, char *uri, char *file_path, handler_t handler)
{
    if (*root_route == NULL)
        _create_new_route(root_route, uri, file_path, handler);
    else
    {
        while ((*root_route)->_next != NULL)
            (*root_route)->_next = (*root_route)->_next->_next;
        _create_new_route(&((*root_route)->_next), uri, file_path, handler);
    }
}

pgn_route_t *pgn_router_get_route(pgn_route_t *root_route, char *uri)
{
    while (root_route != NULL)
    {
        if (strcmp(root_route->uri, uri) == 0)
            return root_route;
        root_route = root_route->_next;
    }
    return NULL;
}

void pgn_router_free(pgn_route_t *root_route)
{
    pgn_route_t *temp;
    while (root_route != NULL)
    {
        temp = (root_route)->_next;
        free((root_route)->uri);
        free((root_route)->file_path);
        free(root_route);
        root_route = temp;
    }
}
