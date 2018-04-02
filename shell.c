#include "common.h"
#include "command.h"
#include "y.tab.h"

command_t command;

int main() {
	while (1) {
		prompt();
		yyparse();
		print(command);
		char a[]="hist";
		if(strcmp(a,command.simple_command[0].arguments[0])==0) {
			print_history();	
		}
		else {
			add_command_to_history(command);
			execute(command);
		}
		command_reset(&command);
	}
}