#include"proclore.h"
void print_process_info(char *next_token) {
    char path[256];
    char exe_path[256]; 
    char process_status[32] = {0};
    char process_group[32] = {0};
    char vm_size[32] = {0};

    // Construct the path to the /proc/<PID>/status file
    snprintf(path, sizeof(path), "/proc/%s/status", next_token);
    FILE *status_file = fopen(path, "r");
    if (status_file == NULL) {
        printf("No such process exists\n");
        return;
    }

    // Read status file line by line to find necessary fields
    char line[1000];
    while (fgets(line, sizeof(line), status_file)) {
        if (strncmp(line, "State:", 6) == 0) {
            sscanf(line, "State:\t%31s", process_status);
        }
        if (strncmp(line, "Tgid:", 5) == 0) {
            sscanf(line, "Tgid:\t%31s", process_group);
        }
        if (strncmp(line, "VmSize:", 7) == 0) {
            sscanf(line, "VmSize:\t%31s", vm_size);
        }
    }
    fclose(status_file);

    // Get the executable path
    snprintf(path, sizeof(path), "/proc/%s/exe", next_token);
    ssize_t len = readlink(path, exe_path, sizeof(exe_path) - 1);
    if (len != -1) {
        exe_path[len] = '\0';  // Null-terminate the string
    } else {
        strcpy(exe_path, "N/A");
    }

    // Print the process information
    printf("PID: %s\nProcess status : %s\nProcess Group : %s\nVirtual Memory : %s KB\nexecutable Path: %s\n", 
           next_token, process_status, process_group, vm_size, exe_path);
}