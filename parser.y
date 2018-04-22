%{

#include "common.h"
#include "command.h"
#include "simple_command.h"
int yylex();
void yyerror(const char *);
void expand_wildcard(char *);

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
arg_list : arg_list WORD    {expand_wildcard($2);}
         |                  {simple_command_reset(&simple_command);}
         ;
cmd_and_args : WORD arg_list    {expand_wildcard($1);}
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

void expand_wildcard(char *arg) {
	 if (strchr(arg, '*') == NULL && strchr(arg, '?') == NULL) {
		insert_argument(&simple_command, arg);
		return;
	 }
	
	regex_t regex;
	char *pat = malloc(100);
	
	// creating pattern for regex.
	char *pat_it = pat;
	*pat_it = '^';
	++pat_it;
	
	char *arg_it = arg;
	while (*arg_it) {
		if (*arg_it == '*') {
			*pat_it = '.';
			++pat_it;
			*pat_it = '*';
			++pat_it;
		} else if (*arg_it == '?') {
			*pat_it = '.';
			++pat_it;
		} else if (*arg_it == '.') {
			*pat_it = '\\';
			++pat_it;
			*pat_it = '.';
			++pat_it;
		} else {
			*pat_it = *arg_it;
			++pat_it;
		}
		++arg_it;
	}
	*pat_it = '$';
	++pat_it;
	*pat_it = 0;
	
	// compile regex.
	if (regcomp(&regex, pat, 0)) {
		perror("regcomp");
	}
	
	// read directory and find matches.
	DIR *dir = opendir(".");
	if (dir == NULL) {
		perror("opendir");
		return;
	}
	struct dirent *ent;
	while ((ent = readdir(dir)) != NULL) {
		if (!regexec(&regex, ent->d_name, 0, NULL, 0)) {
			insert_argument(&simple_command, ent->d_name);	
		}
	}
	closedir(dir);
}
