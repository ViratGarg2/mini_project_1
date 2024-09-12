#include "background.h"

typedef struct proce{
        int pid;
        char name[4096];
        int status;
}proce;

proce back[40000];
int process_count2;
int compare_proce(const void *a, const void *b) {
    const proce *proceA = (const proce *)a;
    const proce *proceB = (const proce *)b;
    
    // Compare the names lexicographically
    return strcmp(proceA->name, proceB->name);
}

void push_process(int pi,char* nam){
  back[process_count2].pid = pi;
 // back[process_count2].name = "";
  strcpy(back[process_count2].name,nam);
  // back[process_count2].name = nam;
  back[process_count2].status = 1;
//  printf("nams is %s\n",back[process_count2].name);
  process_count2++;
 // printf("%d\n",pi);
}
void print_status2(){
   // int process_count2 = sizeof(back) / sizeof(back[0]);
    qsort(back, process_count2, sizeof(proce), compare_proce);
    for(int i = 0;i<process_count2;i++){
      //  printf("siub %s\n",back[i].name);
        if(back[i].status == 1){
            printf("%d: %s Running\n",back[i].pid,back[i].name);
        }
        if(back[i].status == 0){
            printf("%d: %s Stopped\n %d",back[i].pid,back[i].name,back[i].status);
        }
    }
}
void delete_process(int pid){
    int flg = 0;
    int k = 0;
    for(int i = 0;i<process_count2;i++){
        if(back[i].pid == pid){
           // printf("pid found");
            k = i;
            flg = 1;
            break;
        }
    }
   // printf("%d\n",flg);
    for(int i = k;i<process_count2-1 && flg == 1;i++){
        back[i] = back[i+1];
        
    }
    process_count2--;
    if(flg == 0){
       // printf("%sNo such process found%s\n",red,reset);
    }
}

void send_signal_to_pid(int pid, int signal) {
    // Check if the PID exists and is valid
    if (kill(pid, 0) == -1) {
        perror("Error: Invalid PID or process does not exist");
        return;
    }
    if (kill(pid, signal) == 0) {
        // if(signal == 9 || signal == 3 || signal == 4 || signal == 6 || signal == 15 || signal == 11 || signal == 2){
        // delete_process(pid);
        // }
        printf("sent signal %d successfully to process with pid %d\n", signal, pid);
        check_status();
    } else {
        perror("Error sending signal");
    }
}

void check_status(){
    char pid[32];
    for(int i = 0;i<process_count2;i++){
        pid[strcspn(pid, "\n")] = 0;
         char path[256];
        snprintf(path, sizeof(path), "/proc/%d/stat", back[i].pid);
        FILE *stat_file = fopen(path, "r");
        if (stat_file == NULL) {
            delete_process(back[i].pid);
            continue;
        }
        char comm[256], state;
        int ppid;
        fscanf(stat_file, "%d %s %c", &ppid, comm, &state);
        fclose(stat_file);
        const char *status;
        switch (state) {
            case 'R': status = "Running"; back[i].status = 1;break;
            case 'S': status = "Sleeping"; back[i].status = 1;break;
            case 'D': status = "Waiting (Uninterruptible)"; back[i].status = 0;break;
            case 'Z': status = "Zombie (Terminated)";delete_process(back[i].pid); break;
            case 'T': status = "Stopped";back[i].status = 0;printf("stopped process\n"); break;
            default:  status = "Unknown"; break;
        }
    }
}
void bring_to_foreground(int pid) {
    if (kill(pid, 0) == -1) {
        printf("No such process exist\n");
        return;
    }
    kill(pid, SIGCONT);
    tcsetpgrp(STDIN_FILENO, getpgid(pid));
    int status;
    while(waitpid(pid, &status, WUNTRACED) == -1 && errno == EINTR) {
    }
    tcsetpgrp(STDIN_FILENO, getpgid(getpid()));
    check_status();
    if (WIFSTOPPED(status)) {
        printf("Process [%d] stopped\n", pid);
    } else if (WIFEXITED(status)) {
        printf("Process [%d] exited with status %d\n", pid, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Process [%d] killed by signal %d\n", pid, WTERMSIG(status));
    }
}

void bring_to_background(pid_t pid) {
    if (kill(pid, SIGCONT) == -1) {
        perror("Failed to send SIGCONT");
        return;
    }
    check_status();
    printf("Process %d resumed in background\n", pid);
}
void ctrl_d(){

    printf("%s\nCtrl+D detected\n%s",red,reset);
    for(int i = 0;i<process_count2;i++){
        kill(back[i].pid, SIGKILL);
    }
    exit(0);
}
void ctrl_z(int sig){
    push_process(foreground_id,foreground_name2);
    setpgid(foreground_id,0);
    kill(foreground_id, SIGTSTP);
    time2 = 1;
    foreground_id = -1;
    tcsetpgrp(STDIN_FILENO,getpid());
    printf("%d\n",foreground_id);
    check_status();
    printf("%s\nCtxxwrl+Z detected\n%s",red,reset);
}
