#include <stdio.h>
#include "router.h"

void route_index(req_t *req, res_t *res)
{
    fprintf(stderr, "[%s REQUEST] %s\n", req->method == GET_METHOD ? "GET" : "POST", req->uri);
}

void route_hello(req_t *req, res_t *res)
{
    fprintf(stderr, "[%s REQUEST] %s\n", req->method == GET_METHOD ? "GET" : "POST", req->uri);
}

int main()
{
    route_t *root_router = NULL;
    router_add_route(&root_router, "/", &route_index);
    router_add_route(&root_router, "/hello", &route_hello);

    req_t index_request = {GET_METHOD, "/"};
    req_t hello_request = {GET_METHOD, "/hello"};

    router_get_route(root_router, index_request.uri)->handler(&index_request, NULL);
    router_get_route(root_router, hello_request.uri)->handler(&hello_request, NULL);

    router_clear(&root_router);
}