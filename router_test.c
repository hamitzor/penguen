#include <stdio.h>
#include "router.h"

void route_index(char *uri, pgn_res_t *res)
{
    fprintf(stderr, "[%s REQUEST] %s\n", "GET", uri);
}

void route_hello(char *uri, pgn_res_t *res)
{
    fprintf(stderr, "[%s REQUEST] %s\n", "GET", uri);
}

int main()
{
    pgn_route_t *root_router = NULL;
    pgn_router_add_route(&root_router, "/", "public/index.html", &route_index);
    pgn_router_add_route(&root_router, "/hello", "public/hello.html", &route_hello);

    char *index_request = "/";
    char *hello_request = "/hello";

    pgn_router_get_route(root_router, index_request)->handler(index_request, NULL);
    pgn_router_get_route(root_router, hello_request)->handler(hello_request, NULL);

    pgn_router_free(&root_router);
}