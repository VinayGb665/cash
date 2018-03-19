%{

#include<stdio.h>
int yylex();
void yyerror(const char *); 

%}

%union {
    char *str;
};

%token LESS GREAT NEWLINE WORD GREATGREAT PIPE AMPERSAND GREATAMPERSAND GREATGREATAMPERSAND

%%      
goal : command_list
     ;            
arg_list : arg_list WORD
         | 
         ;
cmd_and_args : WORD arg_list
             ;
pipe_list : pipe_list PIPE cmd_and_args
          | cmd_and_args
          ;
io_modifier : GREATGREAT WORD
            | GREAT WORD
            | GREATGREATAMPERSAND WORD
            | GREATAMPERSAND WORD
            | LESS WORD
            ;
io_modifier_list : io_modifier_list io_modifier
                 | 
                 ;
background_optional : AMPERSAND
                    | 
                    ;
command_line : pipe_list io_modifier_list background_optional NEWLINE
             | NEWLINE
             | error NEWLINE{yyerrok;}
             ;
command_list : command_list command_line
             |
             ;


%%