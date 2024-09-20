#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

void searchDirectory(const char *path, const char *filename, int recursive, int case_insensitive);

// Durchsucht eine Datei im angegebenen Verzeichnis
void searchFile(const char *path, const char *filename, int recursive, int case_insensitive) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    // Versucht, das Verzeichnis mit dem angegebenen Pfad zu öffnen
    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        return;
    }

    // Liest Einträge im geöffneten Verzeichnis
    while ((entry = readdir(dir)) != NULL) {
        char full_path[1024];

        // Erstellt den vollständigen Pfad zur Datei oder zum Verzeichnis
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // Holt Informationen über die Datei oder das Verzeichnis
        if (stat(full_path, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        // Überprüft, ob der aktuelle Eintrag ein Verzeichnis ist
        if (S_ISDIR(file_stat.st_mode)) {
            // Verzeichnisse '.' (aktuelles Verzeichnis) und '..' (Übergeordnetes Verzeichnis) werden übersprungen
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                // Wenn die Option -R gesetzt ist, dann rekursive Suche
                if (recursive) {
                    searchDirectory(full_path, filename, recursive, case_insensitive); // Rekursiver Aufruf für Unterverzeichnisse
                }
            }
        }
        // Überprüft, ob der aktuelle Eintrag eine reguläre Datei ist
        else if (S_ISREG(file_stat.st_mode)) {
            // Extrahiert den Dateinamen aus dem vollständigen Pfad
            char *file_name = strrchr(full_path, '/');
            file_name = file_name ? file_name + 1 : full_path;

            // Vergleich des Dateinamens für den übergebenen Dateinamen
            
                if (case_insensitive) {
                    if (strcasecmp(file_name, filename) == 0) {
                        printf("Datei gefunden!\n");
                        printf("%d: %s: %s\n", getpid(), filename, full_path);
                    }
                } else {
                    if (strcmp(file_name, filename) == 0) {
                        printf("Datei gefunden!\n");
                        printf("%d: %s: %s\n", getpid(), filename, full_path);
                    }
                } 
        }
    }
    // Schließt das Verzeichnis, nachdem alle Einträge gelesen wurden
    closedir(dir);
}

// Durchsucht Verzeichnisse rekursiv, falls die -R Option gesetzt ist
void searchDirectory(const char *path, const char *filename, int recursive, int case_insensitive) {
    searchFile(path, filename, recursive, case_insensitive);
}
