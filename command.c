#include "command.h"
#include "common.h"

void prompt() {
	char buf[1024];
	getcwd(buf, 1024);
	struct passwd *p = getpwuid(getuid());
	printf("\033[92m%s\033[0m:\033[94m%s\033[0m$", p->pw_name, buf);
}

void print(command_t command) {
	for (int i = 0; i < command.simple_command_nb; ++i) {
		printf("simple_command(%d): ", i);
		for (int j = 0; j < command.simple_command[i].argument_nb; ++j) {
			printf("%s ", command.simple_command[i].arguments[j]);
		}
		printf("\n");
	}
}

void command_reset(command_t* command) {
	command->simple_command_nb = 0;
	command->in_file[0] = '\0';
	command->out_file[0] = '\0';
	command->background = 0;
}

void execute(command_t command) {
	for (int i = 0; i < command.simple_command_nb; ++i) {
		for (int j = 1, k = command.simple_command[i].argument_nb - 1; j < k; ++j, --k) {
			char tmp_arg[MAX_ARG_LEN];
			strcpy(tmp_arg, command.simple_command[i].arguments[j]);
			strcpy(command.simple_command[i].arguments[j], command.simple_command[i].arguments[k]);
			strcpy(command.simple_command[i].arguments[k], tmp_arg);
		}
	}

	int tmpin = dup(0);
	int tmpout = dup(1);

	int fdin, fdout;
	if (strlen(command.in_file)) {
		fdin = open(command.in_file, O_RDONLY);
	} else {
		fdin = dup(tmpin);
	}

	pid_t pid;
	for (int i = 0; i < command.simple_command_nb; ++i) {
		dup2(fdin, 0);
		close(fdin);

		// last command
		if (i == command.simple_command_nb - 1) {
			if (strlen(command.out_file)) {
				fdout = open(command.out_file, O_WRONLY | O_CREAT);
			} else {
				fdout = dup(tmpout);
			}
		} else {
			int fdpipe[2];
			if (pipe(fdpipe) < 0) {
				perror("pipe");
				_exit(1);
			}
			fdout = fdpipe[1];
			fdin = fdpipe[0];
		}

		dup2(fdout, 1);
		close(fdout);

		// fork and execute
		pid = fork();
		if (pid == 0) {
			execvp(command.simple_command[i].arguments[0], command.simple_command[i].arguments);
			perror("execvp");
			_exit(1);
		} else if (pid < 0) {
			perror("fork");
			_exit(1);
		}
	}
	if (!command.background) {
		waitpid(pid, NULL, 0);
	}

	dup2(tmpin, 0);
	dup2(tmpout, 1);
	close(tmpin);
	close(tmpout);
}

void insert_simple_command(command_t* command, simple_command_t simple_command) {
	command->simple_command[command->simple_command_nb] = simple_command;
	++command->simple_command_nb;
}