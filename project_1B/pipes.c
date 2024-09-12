#include "pipes.h"
#define MAX_CMDS 10
#define MAX_ARGS 10

void handle_redirection(char **args)
{
    int i = 0;

    while (args[i] != NULL)
    {
        if (strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0)
        {
            // Output redirection
            int flags = O_WRONLY | O_CREAT | (strcmp(args[i], ">>") == 0 ? O_APPEND : O_TRUNC);
            int fd = open(args[i + 1], flags, 0644);
            if (fd == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;     // Remove redirection operator
            args[i + 1] = NULL; // Remove file name
        }
        else if (strcmp(args[i], "<") == 0)
        {
            // Input redirection
            int fd = open(args[i + 1], O_RDONLY);
            if (fd == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;     // Remove redirection operator
            args[i + 1] = NULL; // Remove file name
        }
        i++;
    }
}

// Simulate splitting command into arguments (like shell's parsing)
char **split_command(char *command)
{
    char **args = malloc(100 * sizeof(char *));
    char *arg;
    int i = 0;

    arg = strtok(command, " \t\n");
    while (arg != NULL)
    {
        args[i++] = arg;
        arg = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
    return args;
}

// Function to handle pipelines and redirection
void execute_pipe(char *command, char *p, char *c, char *g, int k, int flg, char *save, char *command4)
{
    // printf("siufbas");
    char *commands[MAX_CMDS];
    int num_cmds = 0;
    int pipefd[2];
    int input_fd = 0; // Initially, standard input (stdin)
    pid_t pid;
    // printf("%s",input);
    // Split input string into commands using '|'
    if(command[strlen(command)-2] == '|'){
        printf("%sInvalid command\n%s",red,reset);
        return;
    }
    char *token = strtok(command, "|");
    while (token != NULL && num_cmds < MAX_CMDS)
    {
        commands[num_cmds++] = token;
        token = strtok(NULL, "|");
    }

    // Loop through each command in the pipeline
    // char * commands2[MAX_CMDS];
    //  printf("num_cmds is %d",num_cmds);
    for (int i = 0; i < num_cmds; i++)
    {
        char command3[4096];
        strcpy(command3, commands[i]);
        //  printf("%s oeirhg",command3);
        
        char **args = split_command(commands[i]);
        char command_final[4096];
        for(int i = 0;i<MAX_ARGS;i++){
            if(args[i]!=NULL){
                // printf("args[i] is %s",args[i]);
                // fflush(stdout);
                if(i==0){
                    strcpy(command_final,args[i]);
                   // command_final = args[i];
                }
                else{
                   // printf("argument is %s",args[i]);
                    strcat(command_final," ");
                    strcat(command_final,args[i]);
                   // printf("command_final is %s\n",command_final);
                }
            }
        }
        
        // printf("args[0] is %s",args[0]);
        // fflush(stdout);
        //  printf("commands i is %s\n",commands[i]);
        // If this is not the last command, create a pipe
        if (i < num_cmds - 1)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        // Fork a new process
        // printf("input is %s",command);

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        // printf("arguments are %s",args[0]);
        //  fflush(stdout);
        if (pid == 0)
        {   
            //  printf("input is %s",args[0]);
            // In the child process
            //  printf("args[0] is %s",command3);

            // If not the first command, redirect input from the previous pipe
            if (input_fd != 0)
            {
                dup2(input_fd, STDIN_FILENO);
                //  printf("args[0] is %s",command3);
                //  printf("Hio Kaisa ho? %s",args[0]);

                //  fflush(stdout);
                close(input_fd);
            }

            // If not the last command, redirect output to the current pipe
            // printf("%d",pipefd[0]);
            //  printf("pipefd are %s",args[0]);
            // fflush(stdout);
           // printf("command 3 is %s\n", commands[i]);
           // fflush(stdout);
            if (i < num_cmds - 1)
            {   

                // printf("Hio Kaisa ho? %d %d",pipefd[0],pipefd[1]);
               // fflush(stdout);
                close(pipefd[0]); // Close unused read end
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            // printf("Hio Kaisa ho? %s", args[0]);
            // fflush(stdout);
            // printf("Hio Kaisa ho? %s", args[0]);
            // fflush(stdout);
            //  Handle redirection (<, >)

            // redirecting_input_output()

            handle_redirection(args);
            trim(args[0]);
            
            int a = 0;
           
            if (strncmp(args[0], "reveal", 6) == 0 || strncmp(args[0], "log", 3) == 0 || strncmp(args[0], "hop", 3) == 0 || strncmp(args[0], "proclore", 8) == 0 || strncmp(args[0], "seek", 4) == 0 || strncmp(args[0],"activities",11) == 0)
            {

                input(0, p, c, g, 0, 0, save, command_final);
                fflush(stdout);
                exit(0);
            }
            
            // Execute the command
            // printf("%s",command_final);
            // fflush(stdout);

            if (execvp(args[0], args) == -1)
            {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            fflush(stdout);
        }
        else
        {   
            FILE *pid_file = fopen(pids, "a");
                    if (pid_file != NULL) {
                        fprintf(pid_file, "%d %s\n", pid,command_final); // Write the PID to the file
                        fclose(pid_file);             // Close the file
                    } else {
                        perror("Unable to open file to store PID");
            }
            // In the parent process
            // Close the input file descriptor from the previous command
            if (input_fd != 0)
            {
                close(input_fd);
            }

            // Close the write end of the pipe
            if (i < num_cmds - 1)
            {
                close(pipefd[1]);
                input_fd = pipefd[0]; // Set input for the next command to the read end of the pipe
            }

            // Wait for the child process to finish
            waitpid(pid, NULL, 0);
        }

        // Free dynamically allocated memory
        free(args);
        fflush(stdout);
    }
}