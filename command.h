#ifndef COMMAND_H
#define COMMAND_H

#include "simple_command.h"

#define MAX_FILE_LEN 10
#define SIMPLE_COMMAND_NB 25
#define HISTORY_MAX_SIZE 25


typedef struct {
	int simple_command_nb;
	simple_command_t simple_command[SIMPLE_COMMAND_NB];
	char out_file[MAX_FILE_LEN];
	char in_file[MAX_FILE_LEN];
	char err_file[MAX_FILE_LEN];
	int background;
} command_t;

typedef struct alias{
	char org[100];
	char dup[100];
	int flag;
} alias;

typedef struct alias_t
{
	alias at[100];
	

} alias_t;

static command_t current_command;
static simple_command_t current_simple_command;
static alias_t a_t;
void prompt();
void print(command_t command);
void add_command_to_history(command_t command_t);
void alias_cmd(char *);
void print_history();
void command_reset(command_t* command);
void execute(command_t command);
void insert_simple_command(command_t* command, simple_command_t simple_command);

#endif