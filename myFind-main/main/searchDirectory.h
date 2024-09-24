#ifndef SEARCH_DIRECTORY_H
#define SEARCH_DIRECTORY_H

void searchDirectory(const char *path, const char *filename, int recursive, int case_insensitive, char *msg_buffer, size_t buffer_size, int *found);
void searchFile(const char *path, const char *filename, int recursive, int case_insensitive, char *msg_buffer, size_t buffer_size, int *found);

#endif
