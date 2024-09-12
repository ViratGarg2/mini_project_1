#ifndef PIPES_H
#define PIPES_H
#include "redirection.h"
#include "input.h"
#include "color.h"

char **split_command(char *cmd);
void handle_redirection(char **args);
void execute_pipe(char * str,char*p,char*c,char*g,int k,int flg,char* save,char* command2);

#endif