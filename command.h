#ifndef COMMAND_H
#define COMMAND_H

#include "simple_command.h"

#define MAX_FILE_LEN
#define SIMPLE_COMMAND_NB

struct {
	int simple_command_nb;
	simple_command_t simple_command[SIMPLE_COMMAND_NB];
	char out_file[MAX_FILE_LEN];
	char in_file[MAX_FILE_LEN];
	char err_file[MAX_FILE_LEN];
	int background;
} command_t;

static command_t current_command;
static simple_command_t current_simple_command;

void prompt();
void print();
void clear();
void execute(command_t command);
command_t command();
void insert_simple_command(command_t command, simple_command_t simple_command);

#endif