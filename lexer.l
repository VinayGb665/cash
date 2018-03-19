%{

#include "common.h"
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

">>&"               {return GREATGREATAMPERSAND;}
 
[^ \t\n][^ \t\n]*   {yylval.str = strdup(yytext); return WORD;}
  
%%  