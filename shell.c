#include "common.h"
#include "command.h"
#include "y.tab.h"

command_t command;

int main() {
	while (1) {
		prompt();
		yyparse();
		print(command);
		execute(command);
		command_reset(&command);
	}
}