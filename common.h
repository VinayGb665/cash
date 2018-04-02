// include all packages here
#ifndef COMMON_H
#define COMMON_H

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <fcntl.h>

#define HISTORY_MAX_SIZE 25
#define MAX_ARG 6

char * history[HISTORY_MAX_SIZE][MAX_ARG];
static int history_count;

#endif