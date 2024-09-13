#include "input.h"

void handle_sigchld(int sig)
{
    int status;
    pid_t pid;
    check_status();
    // Reap all child processes that have terminated
    //  if(check(pid)){
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        // check_status();
        if (WIFEXITED(status))
        {
            //  modify_status(pid,0);
            printf("Process %d exited with status %d\n", pid, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            //  modify_status(pid,0);
            printf("Process %d was terminated by signal %d\n", pid, WTERMSIG(status));
        }
        else
        {
            //  modify_status(pid,1);
            printf("Process %d did not exit normally\n", pid);
        }
        // }
    }
}

void read_file(char *path)
{
    FILE *file;
    char line[1024];

    // Open the file in read mode
    file = fopen(path, "r");
    if (file == NULL)
    {
        if (errno == EACCES)
        {
            printf("Missing Permission for Task\n");
            return;
        }
        else
        {
            perror("Error opening file");
        }
    }

    // Read and print each line of the file
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }
    printf("\n");

    // Close the file
    fclose(file);
}
void trim(char *str)
{
    char *start, *end;
    if (str == NULL)
        return;
    start = str;
    while (*start && (isspace(*start) || *start == '\t' || *start == ';'))
    {
        start++;
    }
    end = start + strlen(start) - 1;
    while (end > start && (isspace(*end) || *end == '\t' || *start == ';'))
    {
        end--;
    }
    *(end + 1) = '\0';
    memmove(str, start, end - start + 2);
}

char *trim_spaces(char *str)
{

    // printf("")
    char *start, *end;
    if (str == NULL)
        return NULL;
    // Trim leading spaces and tabs
    start = str;
    while (*start && (isspace(*start) || *start == '\t' || *start == ';'))
    {
        start++;
    }

    // Trim trailing spaces and tabs
    end = start + strlen(start) - 1;
    while (end > start && (isspace(*end) || *end == '\t' || *start == ';' || *end == '\n'))
    {
        end--;
    }

    // Write the new null terminator
    *(end + 1) = '\0';

    // Move the trimmed string to the original pointer
    // memmove(str, start, end - start + 2);
    return str;
}

void input(int file, char *p, char *c, char *g, int k, int flg, char *save, char *command2)
{
    trim(command2);
    if (command2 == NULL)
    {
        printf("%sNo command found%s", red, reset);
    }
    else if (strncmp(command2, "reveal", 6) == 0)
    {
        // printf("executing reveal\n");
        strcpy(time_command, "");
        reveal(p, command2);
    }
    else if (strncmp(command2, "hop", 3) == 0)
    {
        strcpy(time_command, "");
        // printf("hop is %s\n",command2);
        hop(p, command2, c, g);
    }
    else if (strncmp(command2, "log", 3) == 0)
    {
        strcpy(time_command, "");
        char *next_token = strtok(command2, " ");
        next_token = strtok(NULL, " \t\n");
        if (next_token == NULL)
        {
            print_log(file);
        }
        else if (strcmp(next_token, "purge") == 0)
        {
            remove(filepath);
        }
        else if (strcmp(next_token, "execute") == 0)
        {
            char *line = strtok(NULL, " \t\n");
            if (line == NULL)
            {
                printf("No line number specified\n");
            }
            else
            {
                int cnt2 = count_lines_in_file(filepath);
                int line_number = atoi(line);
                if (cnt2 < line_number)
                {
                    printf("Line number out of range\n");
                }
                else
                {
                    char buffer_execute[4096];
                    char *buffer = execute_log(line_number, buffer_execute);
                    strcat(buffer, "\n");
                    int file = open(filepath, O_WRONLY | O_APPEND | O_CREAT, 0644);
                    ssize_t bytes_written = write(file, buffer, strlen(buffer));
                    close(file);
                    char buffer3[1000];
                    input2(buffer, buffer3, file, p, c, g, k, flg);
                }
            }
        }
    }
    else if (strncmp(command2, "proclore", 8) == 0)
    {
        strcpy(time_command, "");
        char *next_token = strtok(command2, " ");
        next_token = strtok(NULL, " ");
        if (next_token == NULL)
        {
            next_token = malloc(12);
            snprintf(next_token, 12, "%d", k);
            next_token[11] = '\0';
        }
        print_process_info(next_token);
    }
    else if (strncmp(command2, "seek", 4) == 0)
    {
        strcpy(time_command, "");
        char *next = strtok(command2, " ");
        next = strtok(NULL, " ");
        char next2[10000];
        char buffer4[10000];
        char cw[10000];
        getcwd(cw, sizeof(cw));
        int dcnt = 0;
        int fcnt = 0;
        int ecnt = 0;
        //   foreground_id = getpid();
        while (next != NULL && next[0] == '-')
        {
            if (next[1] == 'd')
            {
                dcnt = 1;
            }
            else if (next[1] == 'f')
            {
                fcnt = 1;
            }
            else if (next[1] == 'e')
            {
                ecnt = 1;
            }
            next = strtok(NULL, " ");
        }
        if (dcnt == 1 && fcnt == 1)
        {
            printf("%scan't use both flags together\n%s", red, reset);
        }
        strcpy(next2, next);
        next = strtok(NULL, " ");
        int file_count = 0;
        int dir_count = 0;
        char file_path[10000] = "";
        char dir_path[10000] = "";
        if (next == NULL)
        {
            search_dir(cw, next2, buffer4, cw, dcnt, fcnt, ecnt, &file_count, &dir_count, file_path, dir_path, c);
            if (dir_count == 0 && file_count == 0)
            {
                printf("%sNo such file or directory\n%s", red, reset);
            }
            if (ecnt == 1 && (dir_count) == 1 && dcnt == 1)
            {
                chdir(dir_path);
            }
            else if (ecnt == 1 && file_count == 1 && fcnt == 1)
            {
                read_file(file_path);
            }
        }

        else
        {
            search_dir(next, next2, buffer4, cw, dcnt, fcnt, ecnt, &file_count, &dir_count, file_path, dir_path, c);
            if (dir_count == 0 && file_count == 0)
            {
                printf("%sNo such file or directory\n%s", red, reset);
            }
            if (ecnt == 1 && (dir_count) == 1 && dcnt == 1)
            {
                chdir(dir_path);
            }
            else if (ecnt == 1 && file_count == 1 && fcnt == 1)
            {
                read_file(file_path);
            }
        }
    }
    else if (strncmp(command2, "exit", 4) == 0)
    {
        exit(0);
    }
    else if (strncmp(command2, "fg", 2) == 0)
    {
        command2 = strtok(command2, " ");
        command2 = strtok(NULL, " ");
        if (command2)
        {
            int pid3 = atoi(command2);
            bring_to_foreground(pid3);
        }
        else
        {
            printf("%s Missing arguments %s\n", red, reset);
        }
    }

    else if (strncmp(command2, "bg", 2) == 0)
    {
        command2 = strtok(command2, " ");
        command2 = strtok(NULL, " ");
        if (command2)
        {
            int pid3 = atoi(command2);
            bring_to_background(pid3);
        }
        else
        {
            printf("%s Missing arguments %s\n", red, reset);
        }
    }
    else if (strncmp(command2, "activities", 10) == 0)
    {
        print_status2();
    }
    else if (strncmp(command2, "iMan", 4) == 0)
    {
        //  printf("ksjbv");
        char *man_command = strtok(command2, " ");
        man_command = strtok(NULL, " ");
        if (man_command)
        {
            //  printf("man_ command is %s",man_command);
            fetch_and_display_man_page(man_command);
        }
        else
        {
            printf("%sMissing arguments%s\n", red, reset);
        }
    }
    else if (strncmp(command2, "ping", 4) == 0)
    {
        char *ping_command = strtok(command2, " ");
        ping_command = strtok(NULL, " ");
        if (ping_command)
        {
            int pid = atoi(ping_command);
            ping_command = strtok(NULL, " ");
            if (ping_command)
            {
                int signal = atoi(ping_command);
                send_signal_to_pid(pid, signal);
            }
            else
            {
                printf("%s Missing arguments %s\n", red, reset);
            }
        }
        else
        {
            printf("%sMissing arguments%s\n", red, reset);
        }
    }
    else if (strncmp(command2, "neonate", 7) == 0)
    {
        char *neonate_command = strtok(command2, " ");
        if (neonate_command)
        {
            neonate_command = strtok(NULL, " ");
            if (neonate_command && strncmp(neonate_command, "-n", 2) == 0)
            {
                neonate_command = strtok(NULL, " ");
                if (neonate_command)
                {
                    int interval = atoi(neonate_command);
                    last_pid(interval);
                }
                else
                {
                    printf("%sMissing arguments%s\n", red, reset);
                }
            }
            else
            {
                printf("%sMissing arguments or wrong arguments%s\n", red, reset);
            }
        }
        else
        {
            printf("%sMissing arguments%s\n", red, reset);
        }
    }
    else
    {
        trim(command2);
        trim(command2);
        pid_t pid = fork();
        char command3[4096] = " ";
        strcpy(command3, command2);
        if (pid == 0)
        {
            char *argv[4096];
            int argc = 0;
            char *token = strtok(command2, " ");
            while (token != NULL)
            {
                argv[argc++] = token;
                token = strtok(NULL, " ");
            }
            argv[argc] = NULL;
            execvp(argv[0], argv);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)
        {

            foreground_id = pid;
            strcpy(foreground_name2, command3);
            // printf("%s\n",command2);
            char buffer[256];
            struct timespec start, end;
            clock_gettime(CLOCK_MONOTONIC, &start);
            int status;
            while (1)
            {
                pid_t result = waitpid(pid, &status, WNOHANG | WUNTRACED);
                if (result == -1)
                {
                    break;
                }

                if (result == 0)
                {
                    sleep(0.1);
                }
                else if (WIFSTOPPED(status))
                {
                    break;
                }
                else if (WIFEXITED(status) || WIFSIGNALED(status))
                {
                    break;
                }
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            int time3 = elapsed_time;
            // printf("Command '%s' took %d seconds\n",command3, time2);
            if (elapsed_time > 2.0)
            {
                char str[4096];
                sprintf(str, "%d", time3);
                char *tok = strtok(command3, " ");
                //  printf("tok is %s\n",tok);
                // tok = strtok(NULL," ");
                strcpy(time_command, tok);
                strcat(time_command, ":");
                strcat(time_command, str);
                strcat(time_command, "s");
            }
            else
            {
                strcpy(time_command, "");
            }

            // time2 = 1;
        }
        else
        {
            perror("fork failed");
        }
    }
}
void input2(char *buffer2, char *buffer3, int file, char *p, char *c, char *g, int k, int flg)
{
    strcpy(buffer3, buffer2);
    char *save;
    char *command2 = strtok_r(buffer2, ";", &save);
    while (command2 != NULL)
    {
        int saved_stdout = dup(STDOUT_FILENO);
        int saved_stdin = dup(STDIN_FILENO);

        int cnt2 = 0;
        for (int i = 0; i < strlen(command2); i++)
        {
            if (command2[i] == '&')
            {
                cnt2++;
            }
        }
        if (cnt2 == 0)
        {
            int cnt3 = 0;
            int flg2 = 0;
            for (int j = 0; j < strlen(command2); j++)
            {
                if (command2[j] == '|')
                {
                    execute_pipe(command2, p, c, g, 0, 0, "", "");
                    flg2 = 1;
                    fflush(stdout);
                    break;
                }
            }
            if (flg2 == 0)
            {
                command2 = redirecting_input_output(command2, saved_stdout, saved_stdin);
                if (command2 == NULL)
                {
                    // printf("%sNo such input file found!%s\n",red,reset);
                    command2 = strtok_r(NULL, ";", &save);
                    continue;
                }
            }
            else
            {
                command2 = strtok_r(NULL, ";", &save);
                continue;
            }
            selfmade(command2, save, file, p, c, g, k, flg);
        }
        else
        {
            char *command3 = strtok(command2, "&");
            int flg3 = 0;
            while (cnt2 >= 1)
            {
                cnt2--;
                trim(command3);
                pid_t pid = fork();
                int flg5 = 0;
                if (pid == 0)
                {
                    for (int j = 0; j < strlen(command3); j++)
                    {
                        if (command3[j] == '|')
                        {
                            execute_pipe(command3, p, c, g, 0, 0, "", "");
                            flg5 = 1;
                            break;
                        }
                    }
                    if (flg5 == 0)
                    {
                        if (strncmp(command3, "reveal", 6) == 0 || strncmp(command3, "log", 3) == 0 || strncmp(command3, "hop", 3) == 0 || strncmp(command3, "proclore", 8) == 0 || strncmp(command3, "seek", 4) == 0 || strncmp(command3, "activities", 11) == 0 || strncmp(command3, "iMan", 4) == 0 || strncmp(command3, "ping", 3) == 0 || strncmp(command3, "neonate", 7) == 0 || strncmp(command3, "fg", 2) == 0 || strncmp(command3, "bg", 2) == 0)
                        {
                            input(0, p, c, g, 0, 0, save, command3);
                        }
                        else
                        {
                            char *args[] = {"/bin/sh", "-c", command3, NULL};
                            fflush(stdout);
                            execvp(args[0], args);
                        }
                    }
                    exit(1);
                }
                else if (pid > 0)
                {
                    printf("Started background process with PID: %d\n", pid);
                    push_process(pid, command3);
                    struct sigaction sa;
                    sa.sa_handler = &handle_sigchld;
                    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
                    sigaction(SIGCHLD, &sa, 0);
                }
                else
                {
                    perror("fork");
                }
                command3 = strtok(NULL, "&");
            }

            char *last_command = command3;
            if (last_command != NULL)
            {
                // printf("last_command is %s\n",last_command);
                input(file, p, c, g, k, flg, save, last_command);
            }
        }
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdout);
        close(saved_stdin);
        command2 = strtok_r(NULL, ";", &save);
    }
}