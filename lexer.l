%{

#include "common.h"
#include "y.tab.h"  

int bs_flag = 0;

%}

%%  
\n 				{ if (bs_flag) {bs_flag = 0;}  else {return NEWLINE;}}

[ \t]   			{;} 
 
">"     			{bs_flag = 0; return GREAT;} 
 
"<"     			{bs_flag = 0; return LESS;} 
 
">>"    			{bs_flag = 0; return GREATGREAT;} 
 
">&"    			{bs_flag = 0; return GREATAMPERSAND;} 
 
"|"     			{bs_flag = 0; return PIPE;} 
 
"&"     			{bs_flag = 0; return AMPERSAND;} 

">>&"               		{bs_flag = 0; return GREATGREATAMPERSAND;}

"$"	{
	bs_flag = 0;
	return *yytext;
}

"\\"				{bs_flag = 1;}
 
[^ \t\n][^ \t\n]*   		{bs_flag = 0; yylval.str = strdup(yytext); return WORD;}
  
%%  
