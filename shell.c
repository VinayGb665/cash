#include "common.h"
#include "command.h"
#include "y.tab.h"

command_t command;
extern char **environ;

int main(int argc, char *argv[]) {
	
	char ch;
	while (1) {
		if (isatty(STDIN_FILENO)) {
			prompt();
		}
		if ((ch = getc(stdin)) == EOF) {
			break;
		} else {
			ungetc(ch, stdin);
		}
		if (yyparse()) {
			printf("command not recognized\n");
		}
		
		// print(command);
		
		if(strcmp("alias",command.simple_command[0].arguments[0]) == 0){
			alias_cmd(command.simple_command[0].arguments[1]);
		}
		else if(strcmp("hist",command.simple_command[0].arguments[0]) == 0) {
			print_history();
		}
		else {
			add_command_to_history(command);
			execute(command);
		}
		command_reset(&command);
	}

}
