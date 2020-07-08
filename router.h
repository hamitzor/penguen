#ifndef _PENGUEN_ROUTER_H_
#define _PENGUEN_ROUTER_H_

struct req_t;
struct res_t;
struct route_t;

typedef enum request_method_t
{
    GET_METHOD,
    POST_METHOD
} request_method_t;

typedef char *uri_t;
typedef char *host_t;
typedef void (*handler_t)(struct req_t *req, struct res_t *res);
typedef char *header_field;
typedef char *request_body;
typedef unsigned long long int *request_body_len;
typedef char *response_body;
typedef unsigned long long int *response_body_len;

typedef enum http_status_code
{
    HTTP_STATUS_100,
    HTTP_STATUS_101,
    HTTP_STATUS_102,
    HTTP_STATUS_200,
    HTTP_STATUS_201,
    HTTP_STATUS_202,
    HTTP_STATUS_203,
    HTTP_STATUS_204,
    HTTP_STATUS_205,
    HTTP_STATUS_206,
    HTTP_STATUS_207,
    HTTP_STATUS_208,
    HTTP_STATUS_226,
    HTTP_STATUS_300,
    HTTP_STATUS_301,
    HTTP_STATUS_302,
    HTTP_STATUS_303,
    HTTP_STATUS_304,
    HTTP_STATUS_305,
    HTTP_STATUS_307,
    HTTP_STATUS_308,
    HTTP_STATUS_400,
    HTTP_STATUS_401,
    HTTP_STATUS_402,
    HTTP_STATUS_403,
    HTTP_STATUS_404,
    HTTP_STATUS_405,
    HTTP_STATUS_406,
    HTTP_STATUS_407,
    HTTP_STATUS_408,
    HTTP_STATUS_409,
    HTTP_STATUS_410,
    HTTP_STATUS_411,
    HTTP_STATUS_412,
    HTTP_STATUS_413,
    HTTP_STATUS_414,
    HTTP_STATUS_415,
    HTTP_STATUS_416,
    HTTP_STATUS_417,
    HTTP_STATUS_418,
    HTTP_STATUS_421,
    HTTP_STATUS_422,
    HTTP_STATUS_423,
    HTTP_STATUS_424,
    HTTP_STATUS_426,
    HTTP_STATUS_428,
    HTTP_STATUS_429,
    HTTP_STATUS_431,
    HTTP_STATUS_444,
    HTTP_STATUS_451,
    HTTP_STATUS_499,
    HTTP_STATUS_500,
    HTTP_STATUS_501,
    HTTP_STATUS_502,
    HTTP_STATUS_503,
    HTTP_STATUS_504,
    HTTP_STATUS_505,
    HTTP_STATUS_506,
    HTTP_STATUS_507,
    HTTP_STATUS_508,
    HTTP_STATUS_510,
    HTTP_STATUS_511,
    HTTP_STATUS_599
} http_status_code;

typedef enum request_header_field_name
{
    REQ_A_IM,
    REQ_ACCEPT,
    REQ_ACCEPT_CHARSET,
    REQ_ACCEPT_DATETIME,
    REQ_ACCEPT_ENCODING,
    REQ_ACCEPT_LANGUAGE,
    REQ_ACCESS_CONTROL_REQUEST_METHOD,
    REQ_ACCESS_CONTROL_REQUEST_HEADERS,
    REQ_AUTHORIZATION,
    REQ_CACHE_CONTROL,
    REQ_CONNECTION,
    REQ_CONTENT_ENCODING,
    REQ_CONTENT_LENGTH,
    REQ_CONTENT_MD5,
    REQ_CONTENT_TYPE,
    REQ_COOKIE,
    REQ_DATE,
    REQ_EXPECT,
    REQ_FORWARDED,
    REQ_FROM,
    REQ_HOST,
    REQ_HTTP2_SETTINGS,
    REQ_IF_MATCH,
    REQ_IF_MODIFIED_SINCE,
    REQ_IF_NONE_MATCH,
    REQ_IF_RANGE,
    REQ_IF_UNMODIFIED_SINCE,
    REQ_MAX_FORWARDS,
    REQ_ORIGIN,
    REQ_PRAGMA,
    REQ_PROXY_AUTHORIZATION,
    REQ_RANGE,
    REQ_REFERER,
    REQ_TE,
    REQ_TRAILER,
    REQ_TRANSFER_ENCODING,
    REQ_USER_AGENT,
    REQ_UPGRADE,
    REQ_VIA,
    REQ_WARNING,
    request_header_field_name_len
} request_header_field_name;

typedef enum response_header_field_name
{
    RES_ACCESS_CONTROL_ALLOW_ORIGIN,
    RES_ACCESS_CONTROL_ALLOW_CREDENTIALS,
    RES_ACCESS_CONTROL_EXPOSE_HEADERS,
    RES_ACCESS_CONTROL_MAX_AGE,
    RES_ACCESS_CONTROL_ALLOW_METHODS,
    RES_ACCESS_CONTROL_ALLOW_HEADERS,
    RES_ACCEPT_PATCH,
    RES_ACCEPT_RANGES,
    RES_AGE,
    RES_ALLOW,
    RES_ALT_SVC,
    RES_CACHE_CONTROL,
    RES_CONNECTION,
    RES_CONTENT_DISPOSITION,
    RES_CONTENT_ENCODING,
    RES_CONTENT_LANGUAGE,
    RES_CONTENT_LENGTH,
    RES_CONTENT_LOCATION,
    RES_CONTENT_MD5,
    RES_CONTENT_RANGE,
    RES_CONTENT_TYPE,
    RES_DATE,
    RES_DELTA_BASE,
    RES_ETAG,
    RES_EXPIRES,
    RES_IM,
    RES_LAST_MODIFIED,
    RES_LINK,
    RES_LOCATION,
    RES_P3P,
    RES_PRAGMA,
    RES_PROXY_AUTHENTICATE,
    RES_PUBLIC_KEY_PINS,
    RES_RETRY_AFTER,
    RES_SERVER,
    RES_SET_COOKIE,
    RES_STRICT_TRANSPORT_SECURITY,
    RES_TRAILER,
    RES_TRANSFER_ENCODING,
    RES_TK,
    RES_UPGRADE,
    RES_VARY,
    RES_VIA,
    RES_WARNING,
    RES_WWW_AUTHENTICATE,
    RES_X_FRAME_OPTIONS,
    response_header_field_name_len
} response_header_field_name;

typedef struct req_t
{
    request_method_t method;
    uri_t uri;
    host_t host;
    header_field headers[request_header_field_name_len];
    request_body body;
    request_body_len body_len;
} req_t;

typedef struct res_t
{
    http_status_code status;
    header_field headers[response_header_field_name_len];
    response_body body;
    response_body_len body_len;
} res_t;

typedef struct route_t
{
    uri_t uri;
    handler_t handler;
    struct route_t *_next;
} route_t;

void router_add_route(route_t **root_route, uri_t uri, handler_t handler);
route_t * router_get_route(route_t *root_route, uri_t uri);
void router_clear(route_t **root_route);

#endif