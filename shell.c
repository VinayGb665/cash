#include "common.h"
#include "command.h"
#include "y.tab.h"

#define DEB(x)	printf("%s: %d\n", #x, x);

extern int history_count;
extern char* history[HISTORY_MAX_SIZE][MAX_ARG];

command_t command;
extern char **environ;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  //atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int up_arrow() {
	int flag=0;	
	enableRawMode();
	char c;
	c = getc(stdin);
	
	if(c == 27) {
		c = getc(stdin);
		c = getc(stdin);
		// DEB(history_count);
		if(c == 65) {
			flag = 1;
			disableRawMode();
			int j = 0;
			int idx = 0;
			while(history[history_count-1][j+1] != NULL) {
				printf("%s ",history[history_count-1][j]);
				idx = j;
				j++;

			}
			for (int i = idx; i > -1; i--) {
				for (int j = strlen(history[history_count-1][i]) - 1; j > -1; j-- ) {
					ungetc(history[history_count-1][i][j], stdin);
				}
				ungetc(' ',stdin);
			}
		}
	}

	else {
		ungetc(c,stdin);
		disableRawMode();
		printf("%c",c);
		flag = 1;
	}
	return flag;

}

int main(int argc, char *argv[]) {
	
	char ch;
	int flag;
	while (1) {
		if (isatty(STDIN_FILENO)) {
			prompt();
			up_arrow();
		}
		if ((ch = getc(stdin)) == EOF) {
			break;
		} else {
			ungetc(ch, stdin);
		}
		if (yyparse()) {
			printf("command not recognized\n");
			continue;
		}
		
		// print(command);
		
		if(strcmp("alias",command.simple_command[0].arguments[0]) == 0){
			alias_cmd(command.simple_command[0].arguments[1]);
		}
		else if(strcmp("hist",command.simple_command[0].arguments[0]) == 0) {
			print_history();
		}
		else {
			add_command_to_history(command);
			execute(command);
		}
		command_reset(&command);
	}
}
