#include <stdio.h>
#include "http_message.h"
#include <string.h>

int main()
{
    char request[] = "POST / HTTP/1.1\r\nHost: localhost:8000\r\nAccept: text/html\r\n\r\nTarrak";
    req_t req;
    http_parse_request(request, &req);
    return 0;
}