#include "http.h"
#include "conf.h"

int main()
{
    pgn_conf_t conf;
    char addr[] =  "192.168.0.101";
    conf.addr = addr;
    conf.port = 7040;
    pgn_http_server_t *server = pgn_http_create_server(&conf);
    pgn_http_listen(server);
}