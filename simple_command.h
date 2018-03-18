#ifndef SIMPLE_COMMAND_H
#define SIMPLE_COMMAND_H

#define MAX_ARG_NB 10
#define MAX_ARG_LEN 10

struct {
	int argument_nb;
	char arguments[MAX_ARG_NB][MAX_ARG_LEN];
} simple_command_t;

void simple_command();
void insert_argument(char argument[]);

#endif