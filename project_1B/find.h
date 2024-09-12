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
#include <ctype.h>
#include "reveal.h"

extern char PREV_DIR[4096];
void trim_dot(char *path);
void search_dir(char *target_dir, const char *base_name, const char *relative_path, char *cw, int dcnt, int fcnt, int ecnt, int *file_count, int *dir_count, char *file_path, char *dir_path,char *c);
