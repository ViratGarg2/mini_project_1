#include "reveal.h"
#define PATH_MAX 4096

long long total = 0;

void print_file_permissions(mode_t mode,const char* color) {
    char permissions[11];

    permissions[0] = (S_ISDIR(mode)) ? 'd' : '-';
    permissions[0] = (S_ISCHR(mode)) ? 'c' : permissions[0];
    permissions[0] = (S_ISBLK(mode)) ? 'b' : permissions[0];
    permissions[0] = (S_ISFIFO(mode)) ? 'p' : permissions[0];
    permissions[0] = (S_ISLNK(mode)) ? 'l' : permissions[0];
    permissions[0] = (S_ISSOCK(mode)) ? 's' : permissions[0];

    permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';

    printf("%s%s%s",color,permissions,reset);
}

void print_file_info(const char *dir_name, const char *file_name) {
    char full_path[PATH_MAX];
    struct stat file_stat;

    snprintf(full_path, sizeof(full_path), "%s/%s", dir_name, file_name);

    if (stat(full_path, &file_stat) == -1) {
        perror("stat");
        return;
    }

    const char *color;
    if (S_ISDIR(file_stat.st_mode)) {
        color = blue;
    } else if (file_stat.st_mode & S_IXUSR) {
        color = green;
    } else {
        color = white;
    }

    total += file_stat.st_blocks;

    print_file_permissions(file_stat.st_mode, color);
    printf("%s%ld", color, (long)file_stat.st_nlink);
    
    struct passwd *pw = getpwuid(file_stat.st_uid);
    if (pw != NULL) {
        printf(" %s", pw->pw_name);
    } else {
        printf(" %d", file_stat.st_uid);
    }

    struct group *gr = getgrgid(file_stat.st_gid);
    if (gr != NULL) {
        printf(" %s", gr->gr_name);
    } else {
        printf(" %d", file_stat.st_gid);
    }

    printf(" %lld", (long long)file_stat.st_size);

    char time_buf[80];
    struct tm *time_info;
    time_info = localtime(&file_stat.st_mtime);
    strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", time_info);
    printf(" %s", time_buf);

    printf("%s%s%s\n", color, file_name, reset);
}
int compare_entries(const void *a, const void *b) {
    const char *name_a = *(const char **)a;
    const char *name_b = *(const char **)b;
    return strcasecmp(name_a, name_b);
}

void print_detail(char *str, int flg) {
    DIR *dir = opendir(str);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    char *entries[1000];
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (flg == 0 && entry->d_name[0] == '.') {
            continue;
        }
        entries[count] = strdup(entry->d_name);
        count++;
    }

    closedir(dir);

    qsort(entries, count, sizeof(char *), compare_entries);

    for (int i = 0; i < count; i++) {
        print_file_info(str, entries[i]);
        free(entries[i]);
    }
    printf("%s%sTOTAL: %lld\n%s",bold,yellow,total / 2,reset);
    total = 0;
}

void print_less(char *str, int flg) {
    printf("getting executed correctly\n");
    if (str == NULL) {
        printf("string is NULL");
        return;
    }
    DIR *dir = opendir(str);
    if (dir == NULL) {
        printf("invalid path\n");
        return;
    }

    struct dirent *entry;
    char *entries[1000];
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (flg == 0 && entry->d_name[0] == '.') {
            continue;
        }
        entries[count] = strdup(entry->d_name);
        count++;
    }

    closedir(dir);

    qsort(entries, count, sizeof(char *), compare_entries);

    for (int i = 0; i < count; i++) {
        struct stat file_stat;
        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", str, entries[i]);

        if (stat(full_path, &file_stat) == -1) {
            perror("stat");
            free(entries[i]);
            continue;
        }

        const char *color;
        if (S_ISDIR(file_stat.st_mode)) {
            color = blue;
        } else if (file_stat.st_mode & S_IXUSR) {
            color = green;
        } else {
            color = white;
        }

        printf("%s%d %s  %s\n",color, i+1,entries[i], reset);
        free(entries[i]);
    }
}

void get_parent_directory(const char *path, char *parent_dir) {
    char temp_path[1024];
    strncpy(temp_path, path, sizeof(temp_path) - 1);
    temp_path[sizeof(temp_path) - 1] = '\0';

    char *last_slash = strrchr(temp_path, '/');
    
    if (last_slash != NULL) {
        *last_slash = '\0';
    } else {
        strcpy(temp_path, ".");
    }

    strncpy(parent_dir, temp_path, 1024);
}

void reveal(char* p, char* command) {
    // printf("command is %s",command);
    char *l = strtok(command, " ");
  //  printf("l is %s\n",l);
    l = strtok(NULL," ");
    // printf("l2 is %s\n",l);
    char buffer3[4096];
    getcwd(buffer3, sizeof(buffer3));
    if (l == NULL) {
        print_less(buffer3, 0);
        return;
    }

    int flg = 0;
    int acnt = 0;
    int lcnt = 0;

    if (l[0] == '-') {
        for (int i = 0; i < strlen(l); i++) {
            if (l[i] == 'a') acnt++;
            if (l[i] == 'l') lcnt++;
            if (acnt >= 1) flg = 1;
            if (lcnt >= 1 && acnt >= 1) break;
        }
        char *k = strtok(NULL, " ");
        if (k != NULL) {
            if (k[0] == '-') {
                for (int i = 0; i < strlen(k); i++) {
                    if (k[i] == 'a') acnt++;
                    if (k[i] == 'l') lcnt++;
                    if (acnt >= 1) flg = 1;
                    if (acnt >= 1 && lcnt >= 1) break;
                }
                k = strtok(NULL, " ");
                if (k == NULL && lcnt >= 1) {
                    print_detail(buffer3, flg);
                    return;
                } else if (k == NULL && (lcnt < 1 || acnt < 1)) {
                    print_less(buffer3, flg);
                    return;
                }
            }
            if (lcnt >= 1) {
                print_detail(k, flg);
            } else {
                print_less(k, flg);
            }
        } else {
            if (lcnt >= 1) {
                print_detail(buffer3, flg);
            } else {
                print_less(buffer3, flg);
            }
        }
    } else if (strlen(l) == 1 && l[0] == '.') {
        print_less(p, 0);
        return;
    } else if (strcmp(l, "..") == 0) {
        char parent[3000];
        get_parent_directory(p, parent);
        print_less(parent, 0);
        return;
    } else {
        if (lcnt >= 1) {
            print_detail(l, flg);
        } else {
            print_less(l, flg);
        }
    }
}