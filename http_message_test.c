#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http_message.h"
#include "error.h"

int main()
{
    pgn_res_t res;
    res.body = "{ \"value\": 5 }";
    res.body_len = strlen(res.body);
    res.status = STATUS_407;

    fprintf(stderr, "%s\n", pgn_generate_http_response(&res));
    fprintf(stderr, "%ld\n", strlen(pgn_generate_http_response(&res)));

    char request[] = "GET /cgi-bin/process.cgi HTTP/2\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 412\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive\r\n\r\nlicenseID=string&content=string&/paramsXML=string";
    char *uri = pgn_parse_http_request(request);
    if (uri == NULL)
    {
        pgn_print_error();
        return EXIT_FAILURE;
    }
    printf("%s\n", uri);
    return EXIT_SUCCESS;
}