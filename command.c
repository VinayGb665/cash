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

void print_history() {
	
	for (int i = history_count-1; i >= 0; i--) {
		int j=0;
		while(history[i][j]!=NULL){
			printf("%s ",history[i][j]);
			j++;
		}
	}
}

void add_command_to_history( command_t command )
{
  	char pipe[] = "|";
  	if (history_count < HISTORY_MAX_SIZE) {
  		int k=0;
     	for ( int i = 0; i < command.simple_command_nb; i++) {
     		for (int j = 0; j < command.simple_command[i].argument_nb; ++j) {
     			history[history_count][k]=strdup(command.simple_command[i].arguments[j]);
     			k++;
     		}
			history[history_count][k]=strdup(pipe);
			k++;     		
     	}
     	time_t ltime; 
    	ltime=time(NULL); 
    	char * t = asctime(localtime(&ltime));

     	history[history_count][k-1]=strdup(t);
	} 
  	else {
  		for (int index = 1; index < HISTORY_MAX_SIZE; index++) {
  			int j = 0;
  			
  			while(history[index][j]!=NULL) {
  				history[index-1][j]=strdup(history[index][j]);
 	        	j++;
 	        }
 	        
  	 	}
        int k=0;
     	for ( int i = 0; i < command.simple_command_nb; i++) {
     		for (int j = 0; j < command.simple_command[i].argument_nb; ++j) {
     			history[history_count][k]=strdup(command.simple_command[i].arguments[j]);
     			k++;
     		}
			history[history_count][k]=strdup(pipe);
			k++;     		
     	}

		time_t ltime; 
    	ltime=time(NULL); 
    	char * t = asctime(localtime(&ltime));

     	history[HISTORY_MAX_SIZE][k]=strdup(t);
  	}
    history_count++;
}

void command_reset(command_t* command) {
	command->simple_command_nb = 0;
}

void execute(command_t command) {
	pid_t pid;
	for (int i = 0; i < command.simple_command_nb; ++i) {
		pid = fork();
		if (pid == 0) {
			execvp(command.simple_command[i].arguments[0], command.simple_command[i].arguments);
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

void insert_simple_command(command_t* command, simple_command_t simple_command) {
	command->simple_command[command->simple_command_nb] = simple_command;
	++command->simple_command_nb;
}