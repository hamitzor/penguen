#include "router.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void _create_new_route(route_t **route, uri_t uri, handler_t handler)
{
    *route = (route_t *)malloc(sizeof(route_t));
    (*route)->uri = uri;
    (*route)->handler = handler;
    (*route)->_next = NULL;
}

void router_add_route(route_t **root_route, uri_t uri, handler_t handler)
{
    if (*root_route == NULL)
        _create_new_route(root_route, uri, handler);
    else
    {
        while ((*root_route)->_next != NULL)
            (*root_route)->_next = (*root_route)->_next->_next;
        _create_new_route(&((*root_route)->_next), uri, handler);
    }
}

route_t * router_get_route(route_t *root_route, uri_t uri)
{
    while (root_route != NULL)
    {
        if (strcmp(root_route->uri, uri) == 0)
            return root_route;
        root_route = root_route->_next;
    }
    return NULL;
}

void router_clear(route_t **root_route)
{
    route_t *temp;
    while (*root_route != NULL)
    {
        temp = (*root_route)->_next;
        free(*root_route);
        *root_route = temp;
    }
}