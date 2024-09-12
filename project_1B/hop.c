#include"hop.h"
#include"display.h"
#include"input.h"
void hop(char* p,char* command,char *c,char*g){
    // command = trim_spaces(command);
   // trim(command);
    // printf("%s\n",command);
    // char*l = command;
    char* x;
    // printf("command is %s",command);
    x = strtok(command," ");
    x = strtok(NULL," ");
    // printf("")
 // printf("command is oifdn %s",x);
    // char PREV_DIR[10000];
    if(x == NULL){
        printf("%sSome location required\n%s",red,reset);
        return;
    }
    while(x!=NULL){
        // printf("%s",x);
        if(x[0] == '~'){
            getcwd(PREV_DIR,sizeof(PREV_DIR));
            int k = chdir(c);
            if(k!=0){
                perror("error");
                break;
            }
           // printf("hi%s\n",PREV_DIR);
        }
        else if(x[0] == '-'){
            if(PREV_DIR == NULL){
                printf("%sNo previous cd command found\n%s",red,reset);
                return;
            }
            char* temp = (char*)malloc(1000*sizeof(char));
            getcwd(temp,sizeof(temp));
            int k = chdir(PREV_DIR);
            if(k!=0){
                perror("error");
                break;
            }
            strcpy(PREV_DIR,temp);
        }else{
        getcwd(PREV_DIR,sizeof(PREV_DIR));
      // printf("x is %s\n",x);
        int k = chdir(x);
        if(k != 0){
            perror("error");
            break;
        }
        
        }
        x = strtok(NULL," ");
    } 
    hop_display(c,g);
    printf("\n");
}