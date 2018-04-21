%{
	#include <stdio.h>
	int val[256];
	int yylex();
	void yyerror(const char* msg);
%}

%token ID NUM

%right '='
%left '+' '-'
%left '*' '/'

%%
A : S
  | A S
  ;
S : E '\n'	{printf("%d\n", $1);}
  | ID '=' E '\n'	{val[$1] = $3;}
  ;
E : E '+' E 	{$$ = $1 + $3;}
  | E '-' E 	{$$ = $1 - $3;}
  | E '*' E 	{$$ = $1 * $3;}
  | E '/' E 	{$$ = $1 / $3;}
  | '(' E ')' 	{$$ = $2;}
  | ID			{$$ = val[$1];}
  | NUM 		{$$ = $1;}
  ;
%%

void yyerror(const char *msg) {
	printf("error: %s\n", msg);
}

int main() {
	if (!yyparse()) {
		// printf("successful\n");
	} else {
		printf("incorrect syntax\n");
	}
}