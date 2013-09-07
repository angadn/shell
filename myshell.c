/**
 * The Main Program for the shell
 */

#define SHELL_PROMPT "\n=> "
#define CMD_QUIT "quit"

#define ERR_STDIN "%s: Couldn't read from STDIN or exceeded max buffer size of %d\n"

#define BUF_SIZE 1024

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    char *cmd = malloc(BUF_SIZE * sizeof(char));

    do {
        printf("%s", SHELL_PROMPT);
        if (fgets(cmd, BUF_SIZE, stdin) != NULL) {
            //
            // Do stuff here
            //
        } else {
            printf(ERR_STDIN, argv[0], BUF_SIZE);
        }

    } while(strncmp(cmd, CMD_QUIT, strlen(CMD_QUIT)));

    free(cmd);
    return 0;
}