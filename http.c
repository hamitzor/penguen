#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <regex.h>
#include <sys/ioctl.h>
#include "http.h"
#include "error.h"
#include "http_message.h"
#include "log.h"

#define PGN_CHUNK_SIZE 16

extern int pgn_be_verbose;

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
    free(addr);

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
        char *request_uri = NULL;
        struct sockaddr_in server_addr, client_addr;
        socklen_t client_addr_len = 0;

        connfd = accept(server->sockfd, (struct sockaddr *)&client_addr, &client_addr_len);

        if (0 > connfd)
        {
            close(server->sockfd);
            return EXIT_FAILURE;
        }

        int request_len = 0, chunk_num = 0;
        char *buffer = (char *)malloc(sizeof(char) * PGN_CHUNK_SIZE), *temp_request = NULL, *request = NULL;

        fd_set rfds;
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 1000000 / 20;

        FD_ZERO(&rfds);
        FD_SET(connfd, &rfds);

        while (1)
        {
            rc = select(connfd + 1, &rfds, NULL, NULL, &tv);
            if (rc == -1)
            {
                if (request != NULL)
                    free(request);
                close(connfd);
                close(server->sockfd);
                pgn_errorno = PGN_BAD_REQUEST;
                return EXIT_FAILURE;
            }
            else if (rc)
            {
                if (FD_ISSET(connfd, &rfds))
                {
                    request_len = sizeof(char) * PGN_CHUNK_SIZE * (++chunk_num);
                    request = (char *)malloc(request_len);
                    *request = '\0';
                    rc = read(connfd, buffer, PGN_CHUNK_SIZE);
                    buffer[rc] = '\0';
                    if (temp_request != NULL)
                    {
                        strcpy(request, temp_request);
                        free(temp_request);
                    }
                    strcat(request, buffer);
                    temp_request = request;
                    sp = strstr(request, "\r\n\r\n");
                    if (sp != NULL)
                        break;
                }
            }
            else
            {
                if (request != NULL)
                    free(request);
                close(connfd);
                close(server->sockfd);
                pgn_errorno = PGN_BAD_REQUEST;
                return EXIT_FAILURE;
            }
        }

        request_uri = pgn_parse_http_request(request);

        if (pgn_be_verbose)
        {
            fprintf(stdout, "[INFO]\t");
            pgn_log_now();
            char *IP = inet_ntoa(client_addr.sin_addr);
            fprintf(stdout, "\tGET %s Host: %s Port: %d\n", request_uri, IP, client_addr.sin_port);
        }

        if (strstr(request_uri + 1, server->conf->static_dir) == request_uri + 1)
        {
            char *static_file_name = (char *)malloc(sizeof(char) * (strlen(server->conf->base) + strlen(request_uri) + 1));
            *static_file_name = '\0';
            strcat(static_file_name, server->conf->base);
            strcat(static_file_name, request_uri);
            if (access(static_file_name, F_OK) != -1)
            {
                fprintf(stderr, "FILE EXISTS\n");
            }
            else
            {
                fprintf(stderr, "FILE DOES NOT EXIST\n");
            }
        }
        else
        {
            pgn_route_t *router = pgn_router_get_route(server->conf->routes, request_uri);

            if (router != NULL)
            {
                pgn_res_t res;
                char *html_file_name = (char *)malloc(sizeof(char) * (strlen(server->conf->base) + strlen(router->file) + 2));
                *html_file_name = '\0';
                strcat(html_file_name, server->conf->base);
                strcat(html_file_name, "/");
                strcat(html_file_name, router->file);
                FILE *html_file = fopen(html_file_name, "r");
                fseek(html_file, 0, SEEK_END);
                res.body_len = ftell(html_file);
                fseek(html_file, 0, SEEK_SET);
                res.status = STATUS_200;
                char *response = pgn_generate_http_response(&res); //@TODO: MIME types should be added.
                write(connfd, response, strlen(response)); //@TODO: error checking.
                write_file_to_socket(connfd, html_file);
                free(response);
                fclose(html_file);
                free(html_file_name);
            }
            else
            {
                PGN_DEBUG_INFO(fprintf(stdout, "[DEBUG]\t"));
                PGN_DEBUG_INFO(pgn_log_now());
                PGN_DEBUG_INFO(fprintf(stdout, "\tUndefined route is been tried to be accessed: %s\n", request_uri));
            }
        }

        if (shutdown(connfd, SHUT_RDWR) == -1)
        {
            close(connfd);
            close(server->sockfd);
            return EXIT_FAILURE;
        }

        free(request);
        close(connfd);

        PGN_DEBUG_INFO(fprintf(stdout, "[DEBUG]\t"));
        PGN_DEBUG_INFO(pgn_log_now());
        PGN_DEBUG_INFO(fprintf(stdout, "\tConnection is closed\n"));
    }

    close(server->sockfd);

    return EXIT_SUCCESS;
}

int write_file_to_socket(int sockfd, FILE *file)
{
    int rc = PGN_CHUNK_SIZE;
    char *buffer = (char *)malloc(sizeof(char) * PGN_CHUNK_SIZE);
    while (rc == PGN_CHUNK_SIZE)
    {
        rc = fread(buffer, 1, PGN_CHUNK_SIZE, file);
        write(sockfd, buffer, rc);
    }

    return 0;
}