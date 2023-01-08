%{
#include "parser.h"
%}

%option noyywrap
%option yylineno

%%

[ \t\n]                 ;
"@host"                 { return T_HOST; }
"@port"                 { return T_PORT; }
"@enable-gzip"          { return T_ENABLE_GZIP; }
"@base"                 { return T_BASE; }
"@static"               { return T_STATIC; }
"@route"                { return T_ROUTE; }
\"[^\"]+\"              { return T_VAL; }

%%

