#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "http_message.h"
#include "error.h"

const char *pgn_http_version_expr[5] = {"HTTP/0.9", "HTTP/1.0", "HTTP/1.1", "HTTP/2", "HTTP/3"};
const char *pgn_http_status_names[http_status_code_len] = {"100 Continue", "101 Switching Protocols", "102 Processing", "103 Early Hints", "200 OK", "201 Created", "202 Accepted", "203 Non-Authoritative Information", "204 No Content", "205 Reset Content", "206 Partial Content", "207 Multi-Status", "208 Already Reported", "226 IM Used", "300 Multiple Choices", "301 Moved Permanently", "302 Found", "303 See Other", "304 Not Modified", "305 Use Proxy", "306 Switch Proxy", "307 Temporary Redirect", "308 Permanent Redirect", "400 Bad Request", "401 Unauthorized", "402 Payment Required", "403 Forbidden", "404 Not Found", "405 Method Not Allowed", "406 Not Acceptable", "407 Proxy Authentication Required", "408 Request Timeout", "409 Conflict", "410 Gone", "411 Length Required", "412 Precondition Failed", "413 Payload Too Large", "414 URI Too Long", "415 Unsupported Media Type", "416 Range Not Satisfiable", "417 Expectation Failed", "418 I'm a teapot", "420 Method Failure", "421 Misdirected Request", "422 Unprocessable Entity", "423 Locked", "424 Failed Dependency", "425 Too Early", "426 Upgrade Required", "428 Precondition Required", "429 Too Many Requests", "431 Request Header Fields Too Large", "440 Login Time-out", "444 No Response", "449 Retry With", "450 Blocked by Windows Parental Controls", "451 Unavailable For Legal Reasons", "494 Request header too large", "495 SSL Certificate Error", "496 SSL Certificate Required", "497 HTTP Request Sent to HTTPS Port", "498 Invalid Token", "499 Client Closed Request", "500 Internal Server Error", "501 Not Implemented", "502 Bad Gateway", "503 Service Unavailable", "504 Gateway Timeout", "505 HTTP Version Not Supported", "506 Variant Also Negotiates", "507 Insufficient Storage", "508 Loop Detected", "509 Bandwidth Limit Exceeded", "510 Not Extended", "511 Network Authentication Required", "520 Web Server Returned an Unknown Error", "521 Web Server Is Down", "522 Connection Timed Out", "523 Origin Is Unreachable", "524 A Timeout Occurred", "525 SSL Handshake Failed", "526 Invalid SSL Certificate", "527 Railgun Error", "529 Site is overloaded", "530 Site is frozen"};

char *pgn_parse_http_request(char *request)
{
    char *crlf = "\r\n";
    char *sp = " ";
    char *uri;

    char *p = strstr(request, crlf);

    if (p == NULL)
    {
        pgn_errorno = PGN_BAD_REQUEST;
        return NULL;
    }

    *p = '\0';
    p = strstr(request, sp);
    *p = '\0';

    if (strcmp(request, "GET") != 0)
    {
        pgn_errorno = PGN_BAD_REQUEST;
        return NULL;
    }
    request = p + 1;

    p = strstr(request, sp);
    *p = '\0';
    uri = request;

    request = p + 1;

    int valid_version = 0;
    int i;

    for (i = 0; i < 5; i++)
        if (strcmp(request, pgn_http_version_expr[i]) == 0)
            return uri;

    pgn_errorno = PGN_BAD_REQUEST;
    return NULL;
}

char *pgn_generate_http_response(const pgn_res_t *res)
{
    char *crlf = "\r\n";
    unsigned long long int len = 14;

    len += strlen(pgn_http_status_names[res->status]);

    len += res->body_len;
    char *res_string = (char *)malloc(len * sizeof(char));
    memset(res_string, 0, len);
    strcat(res_string, pgn_http_version_expr[2]);
    strcat(res_string, " ");
    strcat(res_string, pgn_http_status_names[res->status]);
    strcat(res_string, crlf);
    strcat(res_string, crlf);
    return res_string;
}
