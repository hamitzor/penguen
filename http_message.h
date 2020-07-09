#ifndef _PENGUEN_HTTP_MESSAGE_H_
#define _PENGUEN_HTTP_MESSAGE_H_

struct req_t;
struct res_t;

typedef enum http_version_t
{
    HTTP_0_9,
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2_0,
    HTTP_3_0,
    http_version_len
} http_version_t;

typedef enum http_method_t
{
    GET_METHOD,
    HEAD_METHOD,
    POST_METHOD,
    PUT_METHOD,
    DELETE_METHOD,
    CONNECT_METHOD,
    OPTIONS_METHOD,
    TRACE_METHOD,
    PATCH_METHOD,
    http_method_len
} http_method_t;

typedef unsigned long long int req_body_len_t;
typedef unsigned long long int res_body_len_t;

typedef enum http_status_code_t
{
    STATUS_100,
    STATUS_101,
    STATUS_102,
    STATUS_103,
    STATUS_200,
    STATUS_201,
    STATUS_202,
    STATUS_203,
    STATUS_204,
    STATUS_205,
    STATUS_206,
    STATUS_207,
    STATUS_208,
    STATUS_226,
    STATUS_300,
    STATUS_301,
    STATUS_302,
    STATUS_303,
    STATUS_304,
    STATUS_305,
    STATUS_306,
    STATUS_307,
    STATUS_308,
    STATUS_400,
    STATUS_401,
    STATUS_402,
    STATUS_403,
    STATUS_404,
    STATUS_405,
    STATUS_406,
    STATUS_407,
    STATUS_408,
    STATUS_409,
    STATUS_410,
    STATUS_411,
    STATUS_412,
    STATUS_413,
    STATUS_414,
    STATUS_415,
    STATUS_416,
    STATUS_417,
    STATUS_418,
    STATUS_420,
    STATUS_421,
    STATUS_422,
    STATUS_423,
    STATUS_424,
    STATUS_425,
    STATUS_426,
    STATUS_428,
    STATUS_429,
    STATUS_431,
    STATUS_440,
    STATUS_444,
    STATUS_449,
    STATUS_450,
    STATUS_451,
    STATUS_494,
    STATUS_495,
    STATUS_496,
    STATUS_497,
    STATUS_498,
    STATUS_499,
    STATUS_500,
    STATUS_501,
    STATUS_502,
    STATUS_503,
    STATUS_504,
    STATUS_505,
    STATUS_506,
    STATUS_507,
    STATUS_508,
    STATUS_509,
    STATUS_510,
    STATUS_511,
    STATUS_520,
    STATUS_521,
    STATUS_522,
    STATUS_523,
    STATUS_524,
    STATUS_525,
    STATUS_526,
    STATUS_527,
    STATUS_529,
    STATUS_530,
    http_status_code_len
} http_status_code_t;

typedef enum req_header_field_name_t
{
    REQ_A_IM,
    REQ_ACCEPT,
    REQ_ACCEPT_CHARSET,
    REQ_ACCEPT_DATETIME,
    REQ_ACCEPT_ENCODING,
    REQ_ACCEPT_LANGUAGE,
    REQ_ACCESS_CONTROL_req_METHOD,
    REQ_ACCESS_CONTROL_req_HEADERS,
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
    req_header_field_name_len
} req_header_field_name_t;

typedef enum res_header_field_name_t
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
    res_header_field_name_len
} res_header_field_name_t;

typedef struct req_header_field_t
{
    req_header_field_name_t name;
    char *value;
} req_header_field_t;

typedef struct res_header_field_t
{
    res_header_field_name_t name;
    char *value;
} res_header_field_t;

typedef struct req_t
{
    http_method_t method;
    char *uri;
    http_version_t version;
    req_header_field_t *headers[req_header_field_name_len];
    char *body;
    req_body_len_t body_len;
} req_t;

typedef struct res_t
{
    http_version_t version;
    http_status_code_t status;
    res_header_field_t *headers[res_header_field_name_len];
    char *body;
    res_body_len_t body_len;
} res_t;

int parse_http_request(char *request, req_t *req);
char * generate_http_response(const res_t *res);
void init_req(req_t *req);
void init_res(res_t *res);

extern const char *req_header_field_names[req_header_field_name_len];
extern const char *res_header_field_names[res_header_field_name_len];
extern const char *http_version_expr[http_version_len];
extern const char *http_methods[http_method_len];
extern const char *http_status_names[http_status_code_len];

#endif