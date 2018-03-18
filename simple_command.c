#include "simple_command.h"
#include "common.h"

simple_command_t simple_command() {
	simple_command_t simple_command;
	return simple_command;
}

void insert_argument(simple_command_t simple_command, char argument[]) {
	strcpy(simple_command.arguments[argument_nb], argument);
	++simple_command.argument_nb;
	simple_command.arguments[argument_nb] = NULL;
}