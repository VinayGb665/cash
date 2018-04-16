%{

#include "common.h"
#include "command.h"
#include "simple_command.h"
int yylex();
void yyerror(const char *);

simple_command_t simple_command;
extern command_t command;

/* not required i guess, delete later.
command_list : command_list command_line
             |
             ;
*/
%}

%union {
    char *str;
};

%token LESS GREAT NEWLINE WORD GREATGREAT PIPE AMPERSAND GREATAMPERSAND GREATGREATAMPERSAND

%type<str>  WORD

%%      
goal : command_line {YYACCEPT;}
     ;            
arg_list : arg_list WORD    {insert_argument(&simple_command, $2);}
         |                  {simple_command_reset(&simple_command);}
         ;
cmd_and_args : WORD arg_list    {insert_argument(&simple_command, $1);}
             ;
pipe_list : pipe_list PIPE cmd_and_args {insert_simple_command(&command, simple_command);}
          | cmd_and_args {insert_simple_command(&command, simple_command);}
          ;
io_modifier : GREATGREAT WORD   {strcpy(command.out_file, $2);}
            | GREAT WORD    {strcpy(command.out_file, $2);}
            | GREATGREATAMPERSAND WORD  {strcpy(command.out_file, $2);}
            | GREATAMPERSAND WORD   {strcpy(command.out_file, $2);}
            | LESS WORD {strcpy(command.in_file, $2);}
            ;
io_modifier_list : io_modifier_list io_modifier
                 | 
                 ;
background_optional : AMPERSAND {command.background = 1;}
                    | 
                    ;
command_line : pipe_list io_modifier_list background_optional NEWLINE
             | NEWLINE
             ;

%%
