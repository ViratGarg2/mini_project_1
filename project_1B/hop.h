#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <time.h>
#include <limits.h>
#include<inttypes.h>
#include<pwd.h>
#include<grp.h>
#include "color.h"

// I am using a global variable which is declared in main.c,we can use it
// directly in hop.c after using extern in .h file
extern char PREV_DIR[4096];
void hop(char*p,char*command,char* c,char* g);
