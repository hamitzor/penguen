#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http_message.h"
#include "error.h"

int main()
{
    res_t res;
    init_res(&res);
    res.body = "{ \"value\": 5 }";
    res.body_len = strlen(res.body);
    res.status = STATUS_407;
    res.version = HTTP_1_1;

    res.headers[0] = (res_header_field_t *)malloc(sizeof(res_header_field_t));
    res.headers[0]->name = RES_ACCEPT_PATCH;
    res.headers[0]->value = "A stupid value 1";

    res.headers[1] = (res_header_field_t *)malloc(sizeof(res_header_field_t));
    res.headers[1]->name = RES_AGE;
    res.headers[1]->value = "Another value.";

    res.headers[2] = (res_header_field_t *)malloc(sizeof(res_header_field_t));
    res.headers[2]->name = RES_CONNECTION;
    res.headers[2]->value = "A value";

    res.headers[3] = (res_header_field_t *)malloc(sizeof(res_header_field_t));
    res.headers[3]->name = RES_TRANSFER_ENCODING;
    res.headers[3]->value = "Yet another value";

    res.headers[4] = (res_header_field_t *)malloc(sizeof(res_header_field_t));
    res.headers[4]->name = RES_CONTENT_LENGTH;
    res.headers[4]->value = "652";

    fprintf(stderr, "%s", generate_http_response(&res));
    fprintf(stderr, "%ld", strlen(generate_http_response(&res)));
    return 0;

    char request[] = "POST /cgi-bin/process.cgi HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 412\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive\r\n\r\nlicenseID=string&content=string&/paramsXML=string";
    req_t req;
    init_req(&req);
    if (parse_http_request(request, &req) == -1)
    {
        penguen_print_error();
        return -1;
    }
    return 0;
}