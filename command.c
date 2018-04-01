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
  	if (history_count < HISTORY_MAX_SIZE) {
     	for (int j = 0; j < command.simple_command[0].argument_nb; ++j) {
     			history[history_count][j]=strdup(command.simple_command[0].arguments[j]);
     	}
     	time_t ltime; 
    	ltime=time(NULL); 
    	char * t = asctime(localtime(&ltime));

     	history[history_count][command.simple_command[0].argument_nb]=strdup(t);
	} 
  	else {
  		for (int index = 1; index < HISTORY_MAX_SIZE; index++) {
  			int j = 0;
  			
  			while(history[index-1][j]!=NULL) {
  				history[index-1][j]=strdup(history[index][j]);
 	        	j++;
 	        }
 	        
  	 	}
        for (int j = 0; j < command.simple_command[0].argument_nb; ++j) {
		 	history[HISTORY_MAX_SIZE][j]=strdup(command.simple_command[0].arguments[j]);
		}
		time_t ltime; 
    	ltime=time(NULL); 
    	char * t = asctime(localtime(&ltime));

     	history[HISTORY_MAX_SIZE][command.simple_command[0].argument_nb]=strdup(t);
  	}
    history_count++;
}


void alias_cmd(char * command){

	int i,count=0;char *x;
	char alias[100],cmd[200];
	 while( (x = strsep(&command,"=")) != NULL ){
	 	count++;
       // printf("%s\n",x);	
        if(count==1){
        	strcpy(alias,x);
        }
        else if(count==2){
        	strcpy(cmd,x);
        }
    }
    if(count==2){
    	for(i=0;i<100;i++){
    		if(a_t.at[i].flag==0){
    			strcpy(a_t.at[i].org,alias);
    			strcpy(a_t.at[i].dup,cmd);
    			a_t.at[i].flag=1;
    			break;
    		}
    	}
    }
    else{
    	printf("Invalid syntax");
    }
     for(i=0;i<100;i++){
     	if(a_t.at[i].flag==0){
     		break;
     	}
     	printf("1%s %s\n",a_t.at[i].dup,a_t.at[i].org);
     	
     }
	/*
		printf("%s ",strsep(&command,"="));
	for(i=0;i<strlen(command);i++){
		if(1==0){
			strcpy(a_t.at[i].org,alias);
			//strcpy(a_t->at[i]->org,command);
		}
	}

*/
}



void command_reset(command_t* command) {
	command->simple_command_nb = 0;
}

void execute(command_t command) {


	pid_t pid;
	for (int i = 0; i < command.simple_command_nb; ++i) {
		pid = fork();
		if (pid == 0) {
			for(int j=0;j<100;j++){
				if(strcmp(a_t.at[j].org,command.simple_command[i].arguments[0])==0){
					//printf("Atleast came till here \n");
					execvp(a_t.at[j].dup, command.simple_command[i].arguments);
					perror("execvp");
					_exit(1);
					break;

				}
			}
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