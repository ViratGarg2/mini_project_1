#include"log.h"
#include<unistd.h>
#include<fcntl.h>
#define BUFFER_SIZE 4096
void history(char* buffer2,int file){
    // printf("hi");
    int stdout_fd = dup(STDOUT_FILENO);
        dup2(file,STDOUT_FILENO);
        strcat(buffer2,"\n");
        printf("%s",buffer2);
        fflush(stdout);
       // cnt++;
        dup2(stdout_fd,STDOUT_FILENO);
}
int count_lines_in_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    int lines = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
    }
    fclose(file);
    return lines;
}

void print_log(int file) {
    cnt = count_lines_in_file(filepath);
    file = open(filepath, O_RDWR | O_CREAT | O_APPEND, 0644);
   //  printf("Number of lines in file are %d",cnt);
  //  printf("jfbg");
       //  printf("more then 15 lines");
        // Print the last 15 lines
        lseek(file, 0, SEEK_END);
        off_t file_size = lseek(file, 0, SEEK_CUR);
        off_t pos = file_size - 1;
        int lines_count = 0;
        char ch;

        // Move the cursor backwards to find the last 15 lines
        while (pos >= 0 && lines_count <= 15) {
            lseek(file, pos, SEEK_SET);
            read(file, &ch, 1);

            if (ch == '\n') {
                lines_count++;
                // printf("yo yoaoidaioncaakcndm\n\n\n");
            }
            pos--;
        }

        // If we found less than 15 lines, start from the beginning
        if (lines_count <= 15) {
            lseek(file, 0, SEEK_SET);
        } else {
            lseek(file, pos + 2, SEEK_SET);  
        }
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(file, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            printf("%s", buffer);
        }
}
void extract_last_line(const char *filename, char *buffer, size_t buffer_size) {
    int file = open(filename, O_RDONLY);
    if (file == -1) {
        perror("Error opening file");
        return;
    }

    off_t file_size = lseek(file, 0, SEEK_END);
    if (file_size == -1) {
        perror("Error seeking file");
        close(file);
        return;
    }

    off_t pos = file_size - 1;
    char ch;
    int line_count = 0;

    while (pos >= 0) {
        lseek(file, pos, SEEK_SET);
        read(file, &ch, 1);

        if (ch == '\n') {
            if (line_count == 1) {
                pos++;
                break;
            }
            line_count++;
        }
        pos--;
    }

    if (pos < 0) {
        pos = 0;
    }

    lseek(file, pos, SEEK_SET);
    ssize_t bytes_read = read(file, buffer, buffer_size - 1);
    if (bytes_read >= 0) {
        buffer[bytes_read] = '\0';
    } else {
        perror("Error reading file");
    }

    close(file);
}
char* execute_log(int line_number,char* buffer_execute){
    int file = open(filepath, O_RDONLY);
    off_t file_size = lseek(file, 0, SEEK_END);
    off_t pos = file_size - 1;
    char ch;
    int newline_found = 0;
    int line = 0;
    while (pos >= 0) {
        lseek(file, pos, SEEK_SET);
        read(file, &ch, 1);

        if (ch == '\n') {
            if(line == line_number+1){
            newline_found = 1;
            break;
            }
            line++;
        }
        pos--;
    }

    if (newline_found) {
        pos++;
    } else {
        pos = 0;
    }
    char buffer[10000];
    lseek(file, pos, SEEK_SET);
    ssize_t bytes_read = read(file, buffer, sizeof(buffer) - 1);
    buffer_execute[bytes_read] = '\0';
    strtok(buffer, "\n");
    strcat(buffer,"\0");
    strcpy(buffer_execute,buffer);
    close(file);
    return buffer_execute;
}