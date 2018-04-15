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
		printf("%d\n",PID[i]);
	}

}

void add_command_to_history( command_t command )
{
	for (int i = 0; i < command.simple_command_nb; ++i) {
		for (int j = 1, k = command.simple_command[i].argument_nb - 1; j < k; ++j, --k) {
			char tmp_arg[MAX_ARG_LEN];
			strcpy(tmp_arg, command.simple_command[i].arguments[j]);
			strcpy(command.simple_command[i].arguments[j], command.simple_command[i].arguments[k]);
			strcpy(command.simple_command[i].arguments[k], tmp_arg);
		}
	}
  	char pipe[] = "|";
  	if (history_count < HISTORY_MAX_SIZE) {
  		int k=0;
	     	for ( int i = 0; i < command.simple_command_nb; i++) {
	     		for (int j = 0; j < command.simple_command[i].argument_nb; ++j) {
	     			history[history_count][k]=strdup(command.simple_command[i].arguments[j]);
	     			k++;
	     		}
	     		//printf("fts jf %d\n", command.simple_command[0].pid);
			history[history_count][k]=strdup(pipe);
			k++;     		
	     	}
	     	time_t ltime; 
	    	ltime=time(NULL); 
	    	char * t = asctime(localtime(&ltime));
	    	t[strlen(t)-1]='\0';	
	     	history[history_count][k-1]=strdup(t);
	     	
	     	//history[history_count][k]=command.simple_command[0].pid;
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
	    	t[strlen(t)-1]='\0';
	     	history[HISTORY_MAX_SIZE][k-1]=strdup(t);
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
     //	printf("1%s %s\n",a_t.at[i].dup,a_t.at[i].org);
     	
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
			//perror("execvp");
			_exit(1);
			
			
		} else if (pid < 0) {
			 
			perror("fork");
			_exit(1);
		}
		else{
			PID[pid_count++]=pid;
			//printf("%d %d\n",PID[c-1],c-1);
			int status;
  		     	wait(&status); 
  		      	if(WIFEXITED(status)==1){
  		      		if(strcmp(command.simple_command[i].arguments[0],"cd")==0){
  		      			chdir(command.simple_command[i].arguments[1]);
  		      		}
  		      	}  
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
