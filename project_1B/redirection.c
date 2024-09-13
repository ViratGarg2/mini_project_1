#include "redirection.h"
void trim2(char *str)
{
    char *start, *end;
    if(str == NULL)return;
    // Trim leading spaces and tabs
    start = str;
    while (*start && (isspace(*start) || *start == '\t' || *start == ';'))
    {
        start++;
    }

    // Trim trailing spaces and tabs
    end = start + strlen(start) - 1;
    while (end > start && (isspace(*end) || *end == '\t' || *start == ';'))
    {
        end--;
    }

    // Write the new null terminator
    *(end + 1) = '\0';

    // Move the trimmed string to the original pointer
    memmove(str, start, end - start + 2);
 //   printf("memov is %s\n",str);
}
char* redirecting_input_output(char * command2,int saved_stdout,int saved_stdin){
   // printf("hi\n");
   char *input_file = NULL;
        char *output_file = NULL;
        int append_mode = 0;

        char for_output[4096];
        strcpy(for_output, command2);
        char *input_pos = strchr(command2, '<');
        if (input_pos) {
            input_file = input_pos + 1;
            *input_pos = '\0'; // Terminate command at '<'
           // printf("input_pos is %s\n",input_pos);
            trim2(input_file);
        }

        // Check for output redirection
        
        char *output_pos = strchr(for_output, '>');
        if (output_pos) {
            output_file = output_pos + 1;
            if (output_pos[1] == '>') {
                append_mode = 1; // ">>" append mode
                output_file += 1; // Move past '>'
            }
           // *output_pos = '\0'; // Terminate command at '>'
            trim2(output_file);
        }

       
        // printf("commands2 is %s\n",command2);
        // Execute command

        // Handle input redirection
        if(output_file && !input_file){
            command2 = strtok(command2, ">");
            trim2(command2);
        }
      //  printf("command 3 is %s",command2);
        if (input_file) {
            input_file = strtok(input_file,"&");
            input_file = strtok(input_file, ">");
            trim2(input_file);
            int input_fd = open(input_file, O_RDONLY | O_EXCL);
            if (input_fd == -1) {
                printf("%sNo such input file found!\n%s",red,reset);
                dup2(saved_stdout, STDOUT_FILENO);
                dup2(saved_stdin, STDIN_FILENO);
                close(saved_stdout);
                close(saved_stdin);
               return NULL;
            }
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        if (output_file) {
            output_file = strtok(output_file,"&");
            trim2(output_file);
            int output_fd;
            if (append_mode) {
                output_fd = open(output_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
            } else {
                output_fd = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            }
            if (output_fd == -1) {
                fprintf(stderr, "Error opening output file: %s\n", strerror(errno));
                dup2(saved_stdout, STDOUT_FILENO);
                dup2(saved_stdin, STDIN_FILENO);
                close(saved_stdout);
                close(saved_stdin);
               return NULL;
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
    return command2;
}
