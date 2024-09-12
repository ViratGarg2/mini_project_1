#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "color.h"

extern int sleepflg;
extern int sleepval;
extern int time2;
extern char time_command[4096];
void display(char*c,char*g);
void hop_display(char*c,char*g);