#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> //für IPC
#include <sys/ipc.h> //für IPC
#include <sys/msg.h> //für IPC
#include <unistd.h> //für fork()
#include <sys/wait.h> //fürs Warten auf Kindprozesse
#include <errno.h> //error handling

#include "parseArguments.h"
#include "searchDirectory.h"

//Nachrichtenstruktur für message queue:
struct message {
    long msg_type; //wird immer auf 1 gesetzt
    char msg_text[1024]; //höchstens 1024 Zeichen
};

int main(int argc, char *argv[]) {
    char *searchpath = NULL;
    char *filenames[100]; //maximal 100 Files
    int recursive_flag = 0;
    int case_insensitive_flag = 0;
    int msgid; //ID von message queue
    
    //Message queue erstellen:
    msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT); //0666 stellt sicher, dass jeder Lese- und Schreibrechte hat
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    parseArguments(argc, argv, &searchpath, filenames, &recursive_flag, &case_insensitive_flag); //ruft getopt auf
    //Funktion hat keinen Rückgabewert, da alle Argumente pointer sind

    printf("Suchpfad: %s\n", searchpath);
    printf("Dateinamen:\n");
    for (int i = 0; filenames[i] != NULL; i++) { //geht alle Dateinamen durch, NULL markiert Ende im Array
        printf("- %s\n", filenames[i]);
    }
    printf("\n");

    for (int i = 0; filenames[i] != NULL; i++) { //geht alle zu suchenden Files durch
        pid_t pid = fork(); //pro File wird ein Kindprozess erstellt
        if (pid == 0) { //pid == 0 -> Kindprozess
            struct message msg;
            msg.msg_type = 1;

            int found = 0; //Speichert, ob Datei gefunden wurde
 

            searchFile(searchpath, filenames[i], recursive_flag, case_insensitive_flag, msg.msg_text, sizeof(msg.msg_text), &found);


            // Wenn keine Datei gefunden wurde, Nachricht anpassen:
            if (!found) {
                snprintf(msg.msg_text, sizeof(msg.msg_text), "%d: Datei nicht gefunden! %s\n", getpid(), filenames[i]);
            }

            // Ergebnis über die Nachrichtenwarteschlange an Elternprozess senden:
            if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
                perror("msgsnd");
            }
        exit(0); // Kindprozess beendet sich
        } else if (pid < 0) { // Fork fehlgeschlagen
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, 0)) > 0) { //Elternprozess wartet auf Kindprozesse
        // Buffer to receive message
        struct message msg;
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
            perror("msgrcv");
        } else { //Nachrichten der Kindprozesse werden ausgegeben:
            printf("%s", msg.msg_text);
        }
    }

    if (pid == -1 && errno != ECHILD) { //wird ausgeführt wenn Fehler (außer kein Kindprozess übrig) auftritt
        perror("waitpid");
    }

    //message queue wird beendet:
    msgctl(msgid, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}
