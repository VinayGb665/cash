#include "command.h"
#include "common.h"

void prompt() {
	// complete
}

void print() {
	// complete
}

void clear() {
	// complete
}

void execute(command_t command) {
	pid_t pid = fork();
	for (int i = 0; i < command.simple_command_nb; ++i) {
		if (pid == 0) {
			execvp(command.simple_command[i].arguments[0], command.simple_command[i]);
			perror("execvp");
			_exit(1);
		} else if (pid < 0) {
			perror("fork");
			return;
		}
	}
	if (!command.background) {
		waitpid(pid, NULL, 0);
	}
}

void insert_simple_command(command_t command, simple_command_t simple_command) {
	command.simple_command[simple_command_nb] = simple_command;
	++simple_command_nb;
}