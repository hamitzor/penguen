#include <stdio.h>
#include "router.h"

void route_index(req_t *req, res_t *res)
{
    fprintf(stderr, "[%s REQUEST] %s from %s\n", req->method == GET_METHOD ? "GET" : "POST", req->uri, req->host);
}

void route_hello(req_t *req, res_t *res)
{
    fprintf(stderr, "[%s REQUEST] %s from %s\n", req->method == GET_METHOD ? "GET" : "POST", req->uri, req->host);
}

int main()
{
    route_t *root_router = NULL;
    router_add_route(&root_router, "/", &route_index);
    router_add_route(&root_router, "/hello", &route_hello);

    req_t index_request = {GET_METHOD, "/", "1.2.3.4"};
    req_t hello_request = {GET_METHOD, "/hello", "5.6.7.8"};

    router_get_route(root_router, index_request.uri)->handler(&index_request, NULL);
    router_get_route(root_router, hello_request.uri)->handler(&hello_request, NULL);

    router_clear(&root_router);
}