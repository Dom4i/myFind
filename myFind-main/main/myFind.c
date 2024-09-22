#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>  // <-- Add this line
#include "parseArguments.h"
#include "searchDirectory.h"

// Struct for message queue
struct message {
    long msg_type;
    char msg_text[1024];
};

int main(int argc, char *argv[]) {
    char *searchpath = NULL;
    char *filenames[100]; // Assumption: Max 100 filenames
    int recursive_flag = 0;
    int case_insensitive_flag = 0;
    int msgid;  // Message queue ID
    
    // Creating the message queue
    msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    parseArguments(argc, argv, &searchpath, filenames, &recursive_flag, &case_insensitive_flag);
    
    printf("Suchpfad: %s\n", searchpath);
    printf("Dateinamen:\n");
    for (int i = 0; filenames[i] != NULL; i++) {
        printf("- %s\n", filenames[i]);
    }
    printf("\n");

    // Fork child process for each filename
    for (int i = 0; filenames[i] != NULL; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            struct message msg;
            msg.msg_type = 1; // Arbitrary message type

            // Perform search in the directory and prepare the message
            if (recursive_flag) {
                searchDirectory(searchpath, filenames[i], recursive_flag, case_insensitive_flag, msg.msg_text, sizeof(msg.msg_text));
            } else {
                searchFile(searchpath, filenames[i], recursive_flag, case_insensitive_flag, msg.msg_text, sizeof(msg.msg_text));
            }

            // Send the result via message queue
            if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
                perror("msgsnd");
            }

            exit(0); // Child process exits
        } else if (pid < 0) {
            // Fork failed
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process waits for child processes to finish and reads from the message queue
    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, 0)) > 0) {
        // Buffer to receive message
        struct message msg;
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
            perror("msgrcv");
        } else {
            printf("%s", msg.msg_text);
        }
    }

    if (pid == -1 && errno != ECHILD) {
        perror("waitpid");
    }

    // Remove message queue
    msgctl(msgid, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}
