#include"find.h"

#define RESET_COLOR "\033[0m"
#define GREEN_COLOR "\033[32m"
#define BLUE_COLOR "\033[34m"

void trim_dot(char *str)
{   
    if(strcmp(str,"..") == 0 || strcmp(str,".") == 0 || strcmp(str,"~") == 0){
        return;
    }
    char *start, *end;
    if(str == NULL)return;
    start = str;
    while (*start && (isspace(*start) || *start == '.'))
    {
        start++;
    }
    end = start + strlen(start) - 1;
    *(end + 1) = '\0';
    memmove(str, start, end - start + 2);
}

void search_dir(char *target_dir, const char *base_name, const char *relative_path, char *cw, int dcnt, int fcnt, int ecnt, int *file_count, int *dir_count, char *file_path, char *dir_path,char *c) {
    DIR *dir;
    struct dirent *entry;
    char path[1024];
    char new_relative_path[1024];
    trim_dot(target_dir);
    if(strcmp(target_dir,"..") == 0){
        char parent_dir[1024];
        get_parent_directory(cw, parent_dir);
        search_dir(parent_dir, base_name, relative_path, cw, dcnt, fcnt, ecnt, file_count, dir_count, file_path, dir_path,c);
        return;
    }
    if (target_dir == NULL) {
        search_dir(cw, base_name, relative_path, cw, dcnt, fcnt, ecnt, file_count, dir_count, file_path, dir_path,c);
        return;
    }
    if(strncmp(target_dir,"~",1) == 0){
        search_dir(c, base_name, relative_path, cw, dcnt, fcnt, ecnt, file_count, dir_count, file_path, dir_path,c);
        return;
    }
    if (!(dir = opendir(target_dir))) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Skip the "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        snprintf(path, sizeof(path), "%s/%s", target_dir, entry->d_name);
        snprintf(new_relative_path, sizeof(new_relative_path), "%s%s/", relative_path, entry->d_name);
        struct stat statbuf;
        if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            if (strncmp(entry->d_name, base_name, strlen(base_name)) == 0) {
                (*dir_count)++;
                if((*dir_count) == 1){
                snprintf(dir_path,10000, "%s%s", relative_path, entry->d_name);
                }
                if((dcnt == 1 && fcnt == 0) || (dcnt == 0 && fcnt == 0)){
                printf("%s%s\n%s",blue,new_relative_path,reset);
                }
            }
            search_dir(path, base_name, new_relative_path, cw, dcnt, fcnt, ecnt, file_count, dir_count, file_path, dir_path,c);
        } else {
            if (strncmp(entry->d_name, base_name, strlen(base_name)) == 0) {
                (*file_count)++;
                if((*file_count) == 1){
                    snprintf(file_path,10000, "%s%s", relative_path, entry->d_name);
                }
                if((fcnt == 1 && dcnt == 0) || (dcnt == 0 && fcnt == 0)){
                    printf("%s%s\n%s",green,new_relative_path,reset);
                }
            }
        }
    }

    closedir(dir);

}