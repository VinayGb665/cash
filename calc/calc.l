%{

#include <stdio.h>
#include <ctype.h>
#include "y.tab.h"

void yyerror(const char *msg);

%}

%%
[a-z]					{yylval = *yytext - 'a'; return ID;}
[0-9]+					{yylval = atoi(yytext); return NUM;}
["+"|"*"|"="|"/"|"\n"|"("|")"|-]	{return *yytext;};
[ \t\r]	;
%%
