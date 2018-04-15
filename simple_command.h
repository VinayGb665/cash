#ifndef SIMPLE_COMMAND_H
#define SIMPLE_COMMAND_H

#define MAX_ARG_NB 10
#define MAX_ARG_LEN 10
#include "common.h"
typedef struct {
	int argument_nb;
	char *arguments[MAX_ARG_NB];
	pid_t pid;
} simple_command_t;

void insert_argument(simple_command_t* simple_command, char argument[]);
void simple_command_reset(simple_command_t* simple_command);
static int PID[25];
int pid_count;

#endif