#include "signals.h"
void handle_sigint(int sig) {
    time2 = 1;
    if(foreground_id == -1){
        printf("%sNo foreground process found\n%s",red,reset);
        return;
    }
    printf("%d\n",foreground_id);
    kill(foreground_id,SIGINT);
    printf("\n%sCtrl+C pressed killing the foreground process\n%s",red,reset);
    printf("%d\n",time2);
}