#include "man.h"
#include "color.h"
void fetch_man_page(char *command) {
    char url[256];
    snprintf(url, sizeof(url), "http://man.he.net/?topic=%s&section=all", command);
    // I have used wget to download html from a given link
    char *args[] = {"wget", "-q", "-O", "/tmp/manpage.html", url, NULL};
    execvp(args[0], args);
    perror("Error: exec failed for wget");
    exit(1);
}

// Function to strip HTML tags from a string
void strip_html_tags(char *str) {
    char *src = str;
    char *dst = str;
    int inside_tag = 0;
    
    while (*src) {
        if (*src == '<') {
            inside_tag = 1;
        } else if (*src == '>') {
            inside_tag = 0;
            src++;
            continue;
        }

        if (!inside_tag) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}
void display_cleaned_man_page() {
    FILE *file = fopen("/tmp/manpage.html", "r");
    // printf("sodbn");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *html_content = malloc(file_size + 1);
    if (html_content == NULL) {
        perror("Memory allocation error");
        exit(1);
    }
    
    fread(html_content, 1, file_size, file);
    fclose(file);
    html_content[file_size] = '\0'; 
    strip_html_tags(html_content);
    char ch;
    int last_was_space = 0;

    for (int i = 0; html_content[i] != '\0'; i++) {
        ch = html_content[i];
        if (ch == '\n' || ch == '\r') {
            if (!last_was_space) {
                putchar('\n'); 
            }
            last_was_space = 0;
        } else if (ch == ' ' || ch == '\t') {
            if (!last_was_space) {
                putchar(' '); 
            }
            last_was_space = 1;
        } else {
            putchar(ch); 
            last_was_space = 0;
        }
    }

    free(html_content);
}


void fetch_and_display_man_page(char *command) {
    pid_t pid;
    // printf("sodbn");
    if ((pid = fork()) == 0) {
        fetch_man_page(command);
    } else if (pid > 0) {
        wait(NULL); 
        if ((pid = fork()) == 0) {
            display_cleaned_man_page();
        } else if (pid > 0) {
            wait(NULL);
        } else {
            printf("%sError fetching man page%s\n",red,reset);
           // perror("Error: fork failed for displaying man page");
        }
    } else {
         printf("%sError fetching man page%s\n",red,reset);
    }
}