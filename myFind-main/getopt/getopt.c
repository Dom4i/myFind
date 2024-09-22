#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

/* Globale Flags */
int recursive_flag = 0;
int case_insensitive_flag = 0;

/* Funktion zur Ausgabe der Usage-Meldung */
void print_usage(char *program_name) {
    fprintf(stderr, "Usage: %s [-R] [-i] <searchpath> <filename1> [filename2 ...]\n", program_name);
    exit(EXIT_FAILURE);
}

/* Main Funktion */
int main(int argc, char *argv[]) {
    int c;
    char *searchpath = NULL;

    /* Verarbeite die Optionen -R und -i */
    while ((c = getopt(argc, argv, "Ri")) != -1) {
        printf("START\n");
        switch (c) {
        case 'R':
            recursive_flag = 1;
            printf("Option -R: Rekursiver Modus aktiviert\n");
            break;
        case 'i':
            case_insensitive_flag = 1;
            printf("Option -i: Groß-/Kleinschreibungsunabhängige Suche aktiviert\n");
            break;
        case '?':
            print_usage(argv[0]);
            exit(EXIT_FAILURE);
        default:
            print_usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    /* Debug Output: Check flags after parsing options */
    printf("DEBUG: recursive_flag = %d, case_insensitive_flag = %d\n", recursive_flag, case_insensitive_flag);

    /* Stelle sicher, dass der Suchpfad angegeben ist */
    if (optind >= argc) {
        fprintf(stderr, "Fehler: Suchpfad fehlt.\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Setze den Suchpfad */
    searchpath = argv[optind++];
    printf("Suchpfad: %s\n", searchpath);

    /* Debug Output: Check for remaining arguments (filenames) */
    if (optind >= argc) {
        fprintf(stderr, "Fehler: Es wurde kein Dateiname angegeben.\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Verarbeite alle angegebenen Dateinamen */
    printf("Dateinamen:\n");
    while (optind < argc) {
        printf("- %s\n", argv[optind++]);
    }

    /* Debug Output: End of the program, everything parsed correctly */
    printf("DEBUG: Argument Parsing abgeschlossen.\n");
    return EXIT_SUCCESS;
}
