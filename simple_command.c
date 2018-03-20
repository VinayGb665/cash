#include "simple_command.h"
#include "common.h"

void insert_argument(simple_command_t* simple_command, char argument[]) {
	simple_command->arguments[simple_command->argument_nb] = malloc(MAX_ARG_LEN);

	for (int i = simple_command->argument_nb; i > 0; --i) {
		strcpy(simple_command->arguments[i], simple_command->arguments[i - 1]);
	}
	strcpy(simple_command->arguments[0], argument);

	++simple_command->argument_nb;
	simple_command->arguments[simple_command->argument_nb] = NULL;
}

void simple_command_reset(simple_command_t* simple_command) {
	simple_command->argument_nb = 0;
}