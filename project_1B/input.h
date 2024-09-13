#ifndef INPUT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include<errno.h>
#include "display.h"
#include "reveal.h"
#include "hop.h"
#include "log.h"
#include "proclore.h"
#include "find.h"
#include "color.h"
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <time.h>
#include <pwd.h>
#include<limits.h>
#include <sys/time.h>
#include "alias.h"
#include "redirection.h"
#include "pipes.h"
#include "man.h"
#include "background.h"
#include "signals.h"
#include "neonate.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
void read_file(char* path);
extern int sleepflg;
extern int sleepval;
extern char PREV_DIR[4096];
extern char* slow_commands[100];
extern char time_command[4096];
extern char* filepath;
extern char* pids;
extern int time2;
extern int foreground_id;
extern char foreground_name2[4096];
void trim(char* str);
char* trim_spaces(char* str);
void input2(char* buffer2,char * buffer3,int file,char*p,char* c,char *g,int k,int flg);
void input(int file,char*p,char* c,char *g,int k,int flg,char* save,char *command2);
#endif