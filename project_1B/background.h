#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <sys/wait.h>
#include <errno.h>
#include"color.h"
#include"input.h"
extern char* pids;
extern int foreground_id;
extern char foreground_name2[4096];
void check_status();
void bring_to_foreground(int pid);
void bring_to_background(pid_t pid);
void push_process(int pid,char* name);
void print_status2();
void delete_process(int pid);
void send_signal_to_pid(int pid, int signal);
void ctrl_d();
void ctrl_z(int sig);
#endif