#include "man.h"
void fetch_man_page(char *command) {
    char url[256];
    snprintf(url, sizeof(url), "http://man.he.net/?topic=%s&section=all", command);

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

// Function to clean and display the man page
void display_cleaned_man_page() {
    FILE *file = fopen("/tmp/manpage.html", "r");
    printf("sodbn");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Read the HTML content into a string
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
    html_content[file_size] = '\0'; // Null-terminate the string

    // Strip HTML tags
    strip_html_tags(html_content);

    // Print the cleaned content
    char ch;
    int last_was_space = 0;

    for (int i = 0; html_content[i] != '\0'; i++) {
        ch = html_content[i];
        if (ch == '\n' || ch == '\r') {
            if (!last_was_space) {
                putchar('\n'); // Print new line only if last char was not space
            }
            last_was_space = 0;
        } else if (ch == ' ' || ch == '\t') {
            if (!last_was_space) {
                putchar(' '); // Print a single space
            }
            last_was_space = 1;
        } else {
            putchar(ch); // Print the character
            last_was_space = 0;
        }
    }

    free(html_content);
}

// Function to fetch and display man page
void fetch_and_display_man_page(char *command) {
    pid_t pid;
    printf("sodbn");
    // Fork to fetch the man page
    if ((pid = fork()) == 0) {
        fetch_man_page(command);
    } else if (pid > 0) {
        wait(NULL);  // Wait for fetch to finish

        // Fork to clean and display the man page
        if ((pid = fork()) == 0) {
            display_cleaned_man_page();
        } else if (pid > 0) {
            wait(NULL);  // Wait for display to finish
        } else {
            perror("Error: fork failed for displaying man page");
        }
    } else {
        perror("Error: fork failed for fetching man page");
    }
}
