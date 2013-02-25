%{
#include <stdio.h>
extern "C" int yylex();
#pragma GCC diagnostic ignored "-Wunused-function"
%}

%%
[0123456789]+           printf("NUMBER\n");
[a-zA-Z][a-zA-Z0-9]*    printf("WORD\n");
%%
