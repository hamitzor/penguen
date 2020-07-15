#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <regex.h>
#include "http.h"
#include "error.h"
#include "http_message.h"

pgn_http_server_t *pgn_http_create_server(pgn_conf_t *conf)
{
    int sockfd;

    pgn_http_server_t *server = (pgn_http_server_t *)malloc(sizeof(pgn_http_server_t));
    server->conf = conf;
    sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    server->sockfd = sockfd;
    if (sockfd == -1)
    {
        pgn_set_error(PGN_CANNOT_CREATE_SERVER);
        return NULL;
    }

    return server;
}

int pgn_http_listen(pgn_http_server_t *server)
{
    struct sockaddr_in server_addr;
    int connfd;
    in_addr_t addr_int = 0;
    char *addr;
    char *sp;
    int rc;
    int octet_val;
    regex_t regex;
    int i;

    regcomp(&regex, "^[[:digit:]]{1,3}([.][[:digit:]]{1,3}){3}$", REG_EXTENDED);

    addr = (char *)malloc(strlen(server->conf->addr) * sizeof(char));
    strcpy(addr, server->conf->addr);

    rc = regexec(&regex, addr, 0, NULL, 0);
    if (rc != 0)
    {
        pgn_set_error(PGN_BAD_IP_ADDRESS);
        return EXIT_FAILURE;
    }

    memset(&server_addr, 0, sizeof server_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server->conf->port);

    sp = addr;

    for (i = 0; i < 4; i++)
    {
        sp = strchr(sp, '.');
        if (i < 3)
        {
            *sp = '\0';
            sp++;
        }
        octet_val = atoi(addr);
        if (octet_val > 255)
        {
            pgn_set_error(PGN_BAD_IP_ADDRESS);
            return EXIT_FAILURE;
        }
        addr_int += octet_val * pow(16, 6 - i * 2);
        addr = sp;
    }

    server_addr.sin_addr.s_addr = htonl(addr_int);

    if (bind(server->sockfd, (struct sockaddr *)&server_addr, sizeof server_addr) == -1)
    {
        close(server->sockfd);
        return EXIT_FAILURE;
    }

    if (listen(server->sockfd, 10) == -1)
    {
        close(server->sockfd);
        return EXIT_FAILURE;
    }

    while (1)
    {
        char *request_uri;
        struct sockaddr_in server_addr, client_addr;
        socklen_t client_addr_len = 0;

        connfd = accept(server->sockfd, (struct sockaddr *)&client_addr, &client_addr_len);

        if (0 > connfd)
        {
            close(server->sockfd);
            return EXIT_FAILURE;
        }

        char request[200];
        ssize_t received = 0;

        received = read(connfd, request, 200);
        request[received] = '\0';

        request_uri = pgn_parse_http_request(request);

        fprintf(stderr, "request uri:\n%s\n", request_uri);

        if (write(connfd, "Hello World!\n", strlen("Hello World!\n")) == -1)
        {
            perror("cannot write to the connection");
            return -1;
        }

        if (shutdown(connfd, SHUT_RDWR) == -1)
        {
            close(connfd);
            close(server->sockfd);
            return EXIT_FAILURE;
        }

        close(connfd);
        fprintf(stderr, "connection is closed.\n");
    }

    close(server->sockfd);

    return EXIT_SUCCESS;
}