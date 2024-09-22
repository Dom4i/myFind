#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

void searchDirectory(const char *path, const char *filename, int recursive, int case_insensitive, char *msg_buffer, size_t buffer_size);

// Searches for a file in the given directory
void searchFile(const char *path, const char *filename, int recursive, int case_insensitive, char *msg_buffer, size_t buffer_size) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        return;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && recursive) {
                searchDirectory(full_path, filename, recursive, case_insensitive, msg_buffer, buffer_size);
            }
        } else if (S_ISREG(file_stat.st_mode)) {
            char *file_name = strrchr(full_path, '/');
            file_name = file_name ? file_name + 1 : full_path;

            if ((case_insensitive && strcasecmp(file_name, filename) == 0) ||
                (!case_insensitive && strcmp(file_name, filename) == 0)) {
                // Fill the message buffer with the required format
                snprintf(msg_buffer, buffer_size, "%d: %s: %s\n", getpid(), filename, full_path);
                closedir(dir); // Close the directory and return
                return;
            }
        }
    }
    closedir(dir);
}

// Recursively searches for files in directories
void searchDirectory(const char *path, const char *filename, int recursive, int case_insensitive, char *msg_buffer, size_t buffer_size) {
    searchFile(path, filename, recursive, case_insensitive, msg_buffer, buffer_size);
}
