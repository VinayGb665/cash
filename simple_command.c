#include "simple_command.h"
#include "common.h"

void insert_argument(simple_command_t simple_command, char argument[]) {
	strcpy(simple_command.arguments[argument_nb], argument);
	++simple_command.argument_nb;
	simple_command.arguments[argument_nb] = NULL;
}