%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "conf.h"
#include "router.h"

pgn_conf_t pgn_conf;

void yyerror(const char *s) { fprintf(stderr,"Error in line %d: %s\n", yylineno,s); exit(EXIT_FAILURE); }
%}

%union{
    char *text;
}

%token T_HOST
%token T_PORT
%token T_ENABLE_GZIP
%token T_BASE
%token T_STATIC
%token T_ROUTE
%token T_VAL

%type<text> value

%start conf

%%

conf    : stmts ;
stmts   : stmt | stmts stmt ;
stmt    : host | port | enable_gzip | base | static | route ;
host    : T_HOST value { pgn_conf.host = $2; } ;
port    : T_PORT value { pgn_conf.port = atoi($2); } ;
enable_gzip : T_ENABLE_GZIP value { pgn_conf.flags |= (strcmp($2, "on") == 0 ? PGN_ENABLE_GZIP : 0); } ;
base    : T_BASE value { pgn_conf.base = $2; } ;
static  : T_STATIC value { pgn_conf.static_dir = $2; } ;
route   : T_ROUTE value value { pgn_router_add_route(&(pgn_conf.routes), $2, $3, NULL); } ;
value   : T_VAL { $$ = (char *)malloc((strlen(yytext) - 2) * sizeof(char)); strncpy($$, yytext + 1, strlen(yytext) - 2); } ;

%%