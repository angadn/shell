/**
 * The Main Program for the shell
 */

#define SHELL_PROMPT "\n=> "

#define CMD_QUIT    "quit"
#define CMD_CD      "cd"
#define CMD_MKDIR   "mkdir"

#define MKDIR_PERM  0700

#define MSG_CD      "Currently in: %s\n"
#define MSG_MKDIR   "Created a new directory at: %s\n"

#define ERR_STDIN       "%s: Couldn't read from STDIN or exceeded max buffer size of %d\n"
#define ERR_NOTFOUND    "Could not locate executable: %s\n"
#define ERR_DEADCHILD   "PID %d terminated unexpectedly\n"
#define ERR_FORK        "Failed to fork!\n"
#define ERR_MKDIR       "Could not create directory at path: %s\n"

#define BUF_SIZE 1024 
#define ARGS_LEN 32

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "tokenize.h"

int changedir(char *);
int makedir(char *);

int main(int argc, char **argv) {
    char *cmd = malloc(BUF_SIZE * sizeof(char));
    char **cmd_args = malloc(ARGS_LEN * sizeof(char *));

    do {
        printf(SHELL_PROMPT);
        if (fgets(cmd, BUF_SIZE, stdin) != NULL) {
            cmd[strlen(cmd)-1] = '\0'; // Drop the \n
            tokenize(cmd, " \t", cmd_args, ARGS_LEN);
            
            if (strncmp(cmd_args[0], CMD_CD, strlen(CMD_CD)) == 0) {
                changedir(cmd_args[1]);
            } else if (strncmp(cmd_args[0], CMD_MKDIR, strlen(CMD_MKDIR)) == 0) {
                makedir(cmd_args[1]);
            } else if (strncmp(cmd_args[0], CMD_QUIT, strlen(CMD_QUIT)) == 0) {
                // Do nothing, we'll be quitting the shell now
            } else {
                pid_t pid = fork();
                if (pid == 0) {
                    // Executed by Child
                    execvp(*cmd_args, cmd_args);
                    // If the following lines are executed,
                    // then the command didn't exist
                    printf(ERR_NOTFOUND, cmd_args[0]);
                    cmd_args[0] = CMD_QUIT;
                } else if (pid > 0) {
                    // Executed by Parent
                    int status;
                    pid_t cpid = wait(&status);

                    status = status & 0x0F; // Last 8 bits of status are 0 for
                                            // successful exit
                    if (status != 0) {
                        printf(ERR_DEADCHILD, cpid);
                    }
                } else {
                    // Failed to fork a Child
                    printf(ERR_FORK);
                }
            }
        } else {
            printf(ERR_STDIN, argv[0], BUF_SIZE);
        }

    } while (strncmp(cmd_args[0], CMD_QUIT, strlen(CMD_QUIT)) != 0);

    free(cmd);
    free(cmd_args);
    return 0;
}

int changedir(char *path) {
    if (path == NULL) {
        printf(MSG_CD, getcwd(path, sizeof(path)));
        free(path);
    } else {
        return chdir(path);
    }

    return 0;
}

int makedir(char *path) {
    if (mkdir(path, MKDIR_PERM) == -1) {
        printf(ERR_MKDIR, path);
    } else {
        printf(MSG_MKDIR, path);
    }

    return 0;
}