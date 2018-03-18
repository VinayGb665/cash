#ifndef COMMAND_H
#define COMMAND_H

#include "simple_command.h"

#define MAX_FILE_LEN

struct {
	simple_command_t simple_command;
	char out_file[MAX_FILE_LEN];
	char in_file[MAX_FILE_LEN];
	char err_file[MAX_FILE_LEN];
	int background;
} command_t;

static command_t current_command;
static simple_command_t current_simple_command;

void prompt();
void print();
void execute();
void clear();
void command();
void insert_simple_command(simple_command_t simple_command);

#endif