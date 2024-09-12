#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "color.h"
extern int time2;
extern char* pids;
extern int foreground_id;
void handle_sigint(int sig);