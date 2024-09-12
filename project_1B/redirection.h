#ifndef REDIRECTION_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>
#include "color.h"
#include <signal.h>
#include <time.h>
#include <sys/time.h>

void trim2(char* str);
char* redirecting_input_output(char* command,int saved_stdout,int saved_stdin);
#endif