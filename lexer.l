%{

#include <string.h> 
#include "y.tab.h"  

%}  
%%  
\n 					{return NEWLINE;}

[ \t]   			{} 
 
">"     			{return GREAT;} 
 
"<"     			{return LESS;} 
 
">>"    			{return GREATGREAT;} 
 
">&"    			{return GREATAMPERSAND;} 
 
“|”     			{return PIPE;} 
 
“&”     			{return AMPERSAND;} 
 
 
[^ \t\n][^ \t\n]*   {yylval.string_val = strdup(yytext); return WORD;} 
  
%%  