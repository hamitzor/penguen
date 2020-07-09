#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "http_message.h"
#include "error.h"

const char *req_header_field_names[req_header_field_name_len] = {"A-IM", "Accept", "Accept-Charset", "Accept-Datetime", "Accept-Encoding", "Accept-Language", "Access-Control-Request-Method", "Access-Control-Request-Headers", "Authorization", "Cache-Control", "Connection", "Content-Encoding", "Content-Length", "Content-MD5", "Content-Type", "Cookie", "Date", "Expect", "Forwarded", "From", "Host", "HTTP2-Settings", "If-Match", "If-Modified-Since", "If-None-Match", "If-Range", "If-Unmodified-Since", "Max-Forwards", "Origin", "Pragma", "Proxy-Authorization", "Range", "Referer", "TE", "Trailer", "Transfer-Encoding", "User-Agent", "Upgrade", "Via", "Warning"};
const char *res_header_field_names[res_header_field_name_len] = {"Access-Control-Allow-Origin", "Access-Control-Allow-Credentials", "Access-Control-Expose-Headers", "Access-Control-Max-Age", "Access-Control-Allow-Methods", "Access-Control-Allow-Headers", "Accept-Patch", "Accept-Ranges", "Age", "Allow", "Alt-Svc", "Cache-Control", "Connection", "Content-Disposition", "Content-Encoding", "Content-Language", "Content-Length", "Content-Location", "Content-MD5", "Content-Range", "Content-Type", "Date", "Delta-Base", "ETag", "Expires", "IM", "Last-Modified", "Link", "Location", "P3P", "Pragma", "Proxy-Authenticate", "Public-Key-Pins", "Retry-After", "Server", "Set-Cookie", "Strict-Transport-Security", "Trailer", "Transfer-Encoding", "Tk", "Upgrade", "Vary", "Via", "Warning", "WWW-Authenticate", "X-Frame-Options"};
const char *http_version_expr[http_version_len] = {"HTTP/0.9", "HTTP/1.0", "HTTP/1.1", "HTTP/2", "HTTP/3"};
const char *http_methods[http_method_len] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"};
const char *http_status_names[http_status_code_len] = {"100 Continue", "101 Switching Protocols", "102 Processing", "103 Early Hints", "200 OK", "201 Created", "202 Accepted", "203 Non-Authoritative Information", "204 No Content", "205 Reset Content", "206 Partial Content", "207 Multi-Status", "208 Already Reported", "226 IM Used", "300 Multiple Choices", "301 Moved Permanently", "302 Found", "303 See Other", "304 Not Modified", "305 Use Proxy", "306 Switch Proxy", "307 Temporary Redirect", "308 Permanent Redirect", "400 Bad Request", "401 Unauthorized", "402 Payment Required", "403 Forbidden", "404 Not Found", "405 Method Not Allowed", "406 Not Acceptable", "407 Proxy Authentication Required", "408 Request Timeout", "409 Conflict", "410 Gone", "411 Length Required", "412 Precondition Failed", "413 Payload Too Large", "414 URI Too Long", "415 Unsupported Media Type", "416 Range Not Satisfiable", "417 Expectation Failed", "418 I'm a teapot", "420 Method Failure", "421 Misdirected Request", "422 Unprocessable Entity", "423 Locked", "424 Failed Dependency", "425 Too Early", "426 Upgrade Required", "428 Precondition Required", "429 Too Many Requests", "431 Request Header Fields Too Large", "440 Login Time-out", "444 No Response", "449 Retry With", "450 Blocked by Windows Parental Controls", "451 Unavailable For Legal Reasons", "494 Request header too large", "495 SSL Certificate Error", "496 SSL Certificate Required", "497 HTTP Request Sent to HTTPS Port", "498 Invalid Token", "499 Client Closed Request", "500 Internal Server Error", "501 Not Implemented", "502 Bad Gateway", "503 Service Unavailable", "504 Gateway Timeout", "505 HTTP Version Not Supported", "506 Variant Also Negotiates", "507 Insufficient Storage", "508 Loop Detected", "509 Bandwidth Limit Exceeded", "510 Not Extended", "511 Network Authentication Required", "520 Web Server Returned an Unknown Error", "521 Web Server Is Down", "522 Connection Timed Out", "523 Origin Is Unreachable", "524 A Timeout Occurred", "525 SSL Handshake Failed", "526 Invalid SSL Certificate", "527 Railgun Error", "529 Site is overloaded", "530 Site is frozen"};

static int _get_string_index(char *str, char **arr, unsigned int arr_length)
{
    int i;
    for (i = 0; i < arr_length; i++)
        if (strcmp(arr[i], str) == 0)
            return i;
    return -1;
}

int parse_http_request(char *request, req_t *req)
{
    int string_index;
    char *crlf = "\r\n";
    char *first_crlf = strstr(request, crlf);

    if (first_crlf == NULL)
    {
        penguen_errorno = PENGUEN_BAD_REQUEST;
        return -1;
    }

    *first_crlf = '\0';

    char *p;
    short i = 0;

    p = strtok(request, " ");

    if (p == NULL)
    {
        penguen_errorno = PENGUEN_BAD_REQUEST;
        return -1;
    }

    while (p != NULL)
    {
        if (i == 0)
        {
            string_index = _get_string_index(p, (char **)http_methods, http_method_len);
            if (string_index == -1)
            {
                penguen_errorno = PENGUEN_BAD_METHOD;
                return -1;
            }
            req->method = string_index;
        }
        else if (i == 1)
            req->uri = p;
        else if (i == 2)
        {
            string_index = _get_string_index(p, (char **)http_version_expr, http_version_len);
            if (string_index == -1)
            {
                penguen_errorno = PENGUEN_BAD_HTTP_VERSION;
                return -1;
            }
            req->version = string_index;
        }
        i += 1;
        p = strtok(NULL, " ");
    }

    if (i != 3)
    {
        penguen_errorno = PENGUEN_BAD_REQUEST;
        return -1;
    }

    request = first_crlf + (2 * sizeof(char));

    char *body = strstr(request, "\r\n\r\n");

    req->body = body == NULL ? NULL : body + (4 * sizeof(char));
    req->body_len = body == NULL ? 0 : strlen(body + (4 * sizeof(char)));

    if (body != NULL)
    {
        *(body - 4) = '\0';
    }

    p = strtok(request, crlf);

    i = 0;
    while (p != NULL)
    {
        char *p2;
        p2 = strstr(p, ": ");

        if (p2 == NULL)
        {
            penguen_errorno = PENGUEN_BAD_HEADER_FIELD;
            return -1;
        }

        *p2 = '\0';
        req->headers[i] = (req_header_field_t *)malloc(sizeof(req_header_field_t));
        req->headers[i]->name = _get_string_index(p, (char **)req_header_field_names, req_header_field_name_len);
        p = p2 + 2;
        req->headers[i]->value = (char *)malloc(strlen(p) * sizeof(char) + 1);
        strcpy(req->headers[i]->value, p);
        p = strtok(NULL, crlf);
        i += 1;
    }
    return 0;
}

char *generate_http_response(const res_t *res)
{
    char *crlf = "\r\n";
    char *header_name_value_seperator = ": ";
    unsigned long long int len = 6;

    len += strlen(http_version_expr[res->version]);
    len += strlen(http_status_names[res->status]);

    int i;
    for (i = 0; i < res_header_field_name_len; i++)
    {
        if (res->headers[i] == NULL)
            break;
        len += strlen(res_header_field_names[res->headers[i]->name]);
        len += strlen(res->headers[i]->value);
        len += 4;
    }

    len += res->body_len;
    char *res_string = (char *)malloc(len * sizeof(char));
    memset(res_string, 0, len);
    strcat(res_string, http_version_expr[res->version]);
    strcat(res_string, " ");
    strcat(res_string, http_status_names[res->status]);
    strcat(res_string, crlf);

    for (i = 0; i < res_header_field_name_len; i++)
    {
        if (res->headers[i] == NULL)
            break;
        strcat(res_string, res_header_field_names[res->headers[i]->name]);
        strcat(res_string, header_name_value_seperator);
        strcat(res_string, res->headers[i]->value);
        strcat(res_string, crlf);
    }
    strcat(res_string, crlf);
    return res_string;
}

void init_req(req_t *req)
{
    int i;
    for (i = 0; i < req_header_field_name_len; i++)
        req->headers[i] = NULL;
}

void init_res(res_t *res)
{
    int i;
    for (i = 0; i < res_header_field_name_len; i++)
        res->headers[i] = NULL;
}