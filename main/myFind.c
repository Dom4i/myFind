#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseArguments.h"
#include "searchDirectory.h"

int main(int argc, char *argv[]) {
    char *searchpath = NULL;
    char *filenames[100]; // Annahme: maximal 100 Dateinamen
    int recursive_flag = 0;
    int case_insensitive_flag = 0;
    
    printf("START\n");

    parseArguments(argc, argv, &searchpath, filenames, &recursive_flag, &case_insensitive_flag);
    
    printf("Suchpfad: %s\n", searchpath);
    printf("Dateinamen:\n");
    for (int i = 0; filenames[i] != NULL; i++) {
        printf("- %s\n", filenames[i]);
    }

    // Durchlaufe die Dateinamen und suche sie
    for (int i = 0; filenames[i] != NULL; i++) {
        if (recursive_flag) {
            searchDirectory(searchpath, filenames[i], recursive_flag, case_insensitive_flag); 
        } else {
            searchFile(searchpath, filenames[i], recursive_flag, case_insensitive_flag);
        }
    }


    return EXIT_SUCCESS;
}
