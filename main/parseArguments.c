#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parseArguments.h"

void parseArguments(int argc, char *argv[], char **searchpath, char *filenames[], int *recursive_flag, int *case_insensitive_flag){
    int c;

    while ((c = getopt(argc, argv, "Ri")) != -1) {
        switch (c) {
            case 'R':
                *recursive_flag = 1;
                break;
            case 'i':
                *case_insensitive_flag = 1;
                break;
            case '?':
                fprintf(stderr, "Usage: %s [-R] [-i] <searchpath> <filename1> [filename2 ...]\n", argv[0]);
                exit(EXIT_FAILURE);
            default:
                fprintf(stderr, "Usage: %s [-R] [-i] <searchpath> <filename1> [filename2 ...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Fehler: Suchpfad fehlt.\n");
        exit(EXIT_FAILURE);
    }

    *searchpath = argv[optind++];

    int index = 0;
    while (optind < argc && index < 99) {
        filenames[index++] = argv[optind++];
    }
    filenames[index] = NULL; // Null-terminierung für die Dateinamen
}


