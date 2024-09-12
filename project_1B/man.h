#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>
void fetch_man_page(char *command);
void display_cleaned_man_page();
void fetch_and_display_man_page(char *command);