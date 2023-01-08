#include <stdio.h>
#include <stdlib.h>
#include "http.h"
#include "conf.h"
#include "parser.h"
#include "lexer.h"
#include "error.h"

int pgn_be_verbose = 0;

int main(int argc, char **argv)
{
    int rc;
    if (argc < 2)
    {
        fprintf(stderr, "Please specify a configuration file!\n");
        exit(EXIT_FAILURE);
    }

    if (argc > 2 && strcmp(argv[2], "-v") == 0)
        pgn_be_verbose = 1;

    yyin = fopen(argv[1], "r");

    pgn_conf.addr = "0.0.0.0";
    pgn_conf.port = 80;
    pgn_conf.flags = 0;
    pgn_conf.routes = NULL;
    pgn_conf.static_dir = NULL;

    yyparse();
    fclose(yyin);

    pgn_http_server_t *server = pgn_http_create_server(&pgn_conf);

    if (server == NULL)
    {
        pgn_print_error();
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Penguen listens for requests at http://%s:%d\n", pgn_conf.addr, pgn_conf.port);
    rc = pgn_http_listen(server);

    if (rc != 0)
    {
        pgn_print_error();
        free(server);
        pgn_conf_free(&pgn_conf);
        exit(EXIT_FAILURE);
    }

    free(server);
    pgn_conf_free(&pgn_conf);
    exit(EXIT_SUCCESS);
}
