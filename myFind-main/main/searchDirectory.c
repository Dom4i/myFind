#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>


// Sucht eine Datei im angegebenen Verzeichnis
void searchFile(const char *path, const char *filename, int recursive, int case_insensitive, char *msg_buffer, size_t buffer_size, int *found) {
    DIR *dir; //Rückgabewert von opendir()
    struct dirent *entry;
    struct stat file_stat;

    if ((dir = opendir(path)) == NULL) { //Falls Verzeichnis nicht geöffnet werden kann:
        perror("opendir");
        return;
    }
    
    while ((entry = readdir(dir)) != NULL) { //liest Dateien und Unterordner ein
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name); //vollständiger Dateipfad wird erstellt

        if (stat(full_path, &file_stat) == -1) {
            perror("stat");
            continue;
        }
        //wenn es sich um Verzeichnis handelt:
        if (S_ISDIR(file_stat.st_mode)) { //ISDIR prüft, ob es sich um Verzeichnis handelt
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && recursive) { //"." und ".." werden ignoriert, damit man nicht in Überordner geht
                searchFile(full_path, filename, recursive, case_insensitive, msg_buffer, buffer_size, found); //neuer Suchaufruf im Unterordner
            }
        //Ansonsten Datei gefunden:
        } else if (S_ISREG(file_stat.st_mode)) { //ISREG prüft, ob es reguläre Datei ist
            char *file_name = strrchr(full_path, '/'); //Pointer wird auf letzten "/" im String gerichtet
            file_name = file_name ? file_name + 1 : full_path; //Pointer wird auf Zeichen nach "/" gerichtet, also Beginn von Filename

            if ((case_insensitive && strcasecmp(file_name, filename) == 0) ||
                (!case_insensitive && strcmp(file_name, filename) == 0)) { //gesuchter und gefundener Dateiname werden verglichen
                // Fülle den Nachrichtenpuffer mit dem gewünschten Format:
                snprintf(msg_buffer, buffer_size, "%d: Datei gefunden! %s: %s\n", getpid(), filename, full_path);
                *found = 1;  //gespeichert, dass Datei gefunden wurde
                closedir(dir); //Verzeichnis schließen
                return;
            }
        }
    }
    closedir(dir);
}

