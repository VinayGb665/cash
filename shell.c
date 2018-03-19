#include "common.h"
#include "command.h"

command_t command;

int main() {
	while (1) {
		printf("$");
		yyparse();
		execute(command);
	}
}