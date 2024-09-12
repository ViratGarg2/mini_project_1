#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/types.h>
#include "color.h"

void get_parent_directory(const char* path,char* parent_dir);
void print_file_permissions(mode_t mode,const char* color);
void print_less(char*str,int flg);
void print_detail(char* str,int flg);
void reveal(char*p,char* command);
