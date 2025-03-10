#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "signals.h"

int main() {
    char *line;

    setup_signal_handlers();

    while (1) {
        line = readline("minishell> ");

        if (!line) {
            printf("exit\n");
            break;
        }

        if (strlen(line) > 0) {
            add_history(line);
            // Process the line (e.g., execute commands)
            printf("You entered: %s\n", line); // Replace with your command execution logic
        }

        free(line);
    }

    return 0;
}
