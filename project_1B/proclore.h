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

void print_process_info(char* next_token);
