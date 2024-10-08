#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //für getopt()
#include <string.h>
#include "parseArguments.h" //header

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
            case '?': //Ungültige Option:
                fprintf(stderr, "Usage: %s [-R] [-i] <searchpath> <filename1> [filename2 ...]\n", argv[0]);
                exit(EXIT_FAILURE);
            default:
                fprintf(stderr, "Usage: %s [-R] [-i] <searchpath> <filename1> [filename2 ...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) { //argc (counter) muss höher als optind sein, da ansonsten kein Suchpfad angegeben wurde
        fprintf(stderr, "Fehler: Suchpfad fehlt.\n");
        exit(EXIT_FAILURE);
    }

    *searchpath = argv[optind++]; //Speichern vom Suchpfad

    int index = 0;
    while (optind < argc && index < 99) { //Filenames speichern: maximal 100 (siehe myFind.c)
        filenames[index++] = argv[optind++];
    }
    filenames[index] = NULL; // Ende des Arrays wild mit NULL gekennzeichnet
}