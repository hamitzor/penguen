#include <stdio.h>
#include <string.h>
#include "http_message.h"

const char *req_header_field_names[req_header_field_name_len] = {"A-IM", "Accept", "Accept-Charset", "Accept-Datetime", "Accept-Encoding", "Accept-Language", "Access-Control-Request-Method", "Access-Control-Request-Headers", "Authorization", "Cache-Control", "Connection", "Content-Encoding", "Content-Length", "Content-MD5", "Content-Type", "Cookie", "Date", "Expect", "Forwarded", "From", "Host", "HTTP2-Settings", "If-Match", "If-Modified-Since", "If-None-Match", "If-Range", "If-Unmodified-Since", "Max-Forwards", "Origin", "Pragma", "Proxy-Authorization", "Range", "Referer", "TE", "Trailer", "Transfer-Encoding", "User-Agent", "Upgrade", "Via", "Warning"};
const char *res_header_field_names[res_header_field_name_len] = {"Access-Control-Allow-Origin", "Access-Control-Allow-Credentials", "Access-Control-Expose-Headers", "Access-Control-Max-Age", "Access-Control-Allow-Methods", "Access-Control-Allow-Headers", "Accept-Patch", "Accept-Ranges", "Age", "Allow", "Alt-Svc", "Cache-Control", "Connection", "Content-Disposition", "Content-Encoding", "Content-Language", "Content-Length", "Content-Location", "Content-MD5", "Content-Range", "Content-Type", "Date", "Delta-Base", "ETag", "Expires", "IM", "Last-Modified", "Link", "Location", "P3P", "Pragma", "Proxy-Authenticate", "Public-Key-Pins", "Retry-After", "Server", "Set-Cookie", "Strict-Transport-Security", "Trailer", "Transfer-Encoding", "Tk", "Upgrade", "Vary", "Via", "Warning", "WWW-Authenticate", "X-Frame-Options"};
const char *http_version_literals[http_version_len] = {"HTTP/0.9", "HTTP/1.0", "HTTP/1.1", "HTTP/2", "HTTP/3"};
const char *http_methods[http_method_len] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"};

static int _get_string_index(char *str, char **arr, unsigned int arr_length)
{
    int i;
    for (i = 0; i < arr_length; i++)
        if (strcmp(arr[i], str) == 0)
            return i;
    return -1;
}

int http_parse_request(char *request, req_t *req)
{
    char *crlf = "\r\n";
    char *first_crlf = strstr(request, crlf);
    *first_crlf = '\0';
    char *start_line = request;

    char *p;
    short i = 0;

    p = strtok(start_line, " ");
    while (p != NULL)
    {
        if (i == 0)
            req->method = _get_string_index(p, (char **)http_methods, http_method_len);
        else if (i == 1)
            req->uri = p;
        else if (i == 2)
            req->version = _get_string_index(p, (char **)http_version_literals, http_version_len);
        i += 1;
        p = strtok(NULL, " ");
    }

    request = first_crlf + (2 * sizeof(char));

    char *body = strstr(request, "\r\n\r\n") + (4 * sizeof(char));
    req->body = body;
    req->body_len = strlen(body);
    *(body - 4) = '\0';

    p = strtok(request, crlf);

    while (p != NULL)
    {
        printf("%s\n", p);
        p = strtok(NULL, crlf);
    }

    return 0;
}