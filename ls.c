#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void printPermissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

void listFiles(const char* dirPath, int recursive, int displayDetails) {
    DIR* dir = opendir(dirPath);
    if (dir == NULL) {
        fprintf(stderr, "Cannot open directory '%s': %s\n", dirPath, strerror(errno));
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            if (recursive) {
                char path[1024];
                snprintf(path, sizeof(path), "%s/%s", dirPath, entry->d_name);
                printf("%s\n", path);
                listFiles(path, recursive, displayDetails);
            }
        }
        else {
            if(size){
                printf("%s/%s %lld\n", dirPath, entry->d_name,(long long)fileStat.st_blocks);
            }else{
                printf("%s/%s\n", dirPath, entry->d_name);
            }          
        }

        if (displayDetails) {
            char filePath[1024];
            snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, entry->d_name);

            struct stat fileStat;
            if (stat(filePath, &fileStat) < 0) {
                fprintf(stderr, "Cannot get file information for '%s': %s\n", filePath, strerror(errno));
                continue;
            }

            struct passwd* pw = getpwuid(fileStat.st_uid);
            struct group* gr = getgrgid(fileStat.st_gid);
            char dateModified[20];
            strftime(dateModified, sizeof(dateModified), "%Y-%m-%d %H:%M:%S", localtime(&fileStat.st_mtime));
            printPermissions(fileStat.st_mode);
            if(!owner){
                printf("  %s", pw->pw_name);
            }
            if(!group){
               printf("  %s", gr->gr_name); 
            }
            printf("  %ld", fileStat.st_size);
            printf("  %s ", dateModified);
        }
    }
    printf("\n");
    closedir(dir);
}

int main(int argc, char* argv[]) {
    int recursive = 0;
    int displayDetails = 0;
    const char* dirPath = ".";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            displayDetails = 1;
        }
        else if (strcmp(argv[i], "-o") == 0) {
            group = 1;
        }
        else if (strcmp(argv[i], "-g") == 0) {
            owner = 1;
        }
        else if (strcmp(argv[i], "-s") == 0) {
            size = 1;
        }
        else if (strcmp(argv[i], "-R") == 0) {
            recursive = 1;
        }
        else {
            dirPath = argv[i];
        }
    }

    listFiles(dirPath, recursive, displayDetails);

    return 0;
}
