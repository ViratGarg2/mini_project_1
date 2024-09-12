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

extern int cnt;
extern char * filepath;
void extract_last_line(const char *filename,char* buffer,size_t size);
void history(char* buffer2,int file);
void print_log(int file);
int count_lines_in_file(const char* filename);
char* execute_log(int line_number,char* buffer_execute);

