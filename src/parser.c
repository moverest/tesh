#include "parser.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tools.h"


void static close_all_pipes(pipe_t *pipes, int size) {
    for (size_t i = 0; i < size; i++) {
        close(pipes[i].fd[0]);
        close(pipes[i].fd[1]);
    }
}


int exec_statements(statements_t *t) {
    int         count_cmd;
    pid_t       *sons;
    int         status;
    statement_t st;
    bool        temp1;
    bool        temp2;

    for (size_t i = 0; i < t->num_statements; i++) { // Pour chaque statement_t
        st        = t->statements[i];
        count_cmd = st.num_commands;
        pipe_t pipes[count_cmd - 1];

        for (size_t i = 0; i < count_cmd - 1; i++) {
            pipe(pipes[i].fd);
        }

        sons = (int *)malloc(sizeof(int) * count_cmd);

        for (size_t j = 0; j < count_cmd; j++) { // Pour chaque command_t
            pid_t current_pid = fork();
            if (current_pid == 0) {              // On est dans le fils
                if (j > 0) {                     // Si ce n'est pas la premiere commande de la chaine
                    // On redirige l'entrée
                    dup2(pipes[j - 1].fd[0], 0);
                }
                if (j < count_cmd - 1) { // Si ce n'est pas la derniere commande de la chaine
                    // On redirige la sortie
                    dup2(pipes[j].fd[1], 1);
                }

                close_all_pipes(pipes, count_cmd - 1);

                execvp(st.cmds[j].argv[0], st.cmds[j].argv);
            } else {
                sons[j] = current_pid;
            }
        }

        close_all_pipes(pipes, count_cmd - 1);

        for (size_t j = 0; j < count_cmd; j++) {
            //printf("Pid du fils qu'on attend : %d\n", (int) sons[j]);
            waitpid(sons[j], &status, 0);
        }/**/
        //waitpid(sons[count_cmd-1], &status, 0);
        free(sons);
        temp1 = (WIFEXITED(status) && (WEXITSTATUS(status) != 0) && st.go_on_condition == GO_ON_IF_FAILURE);
        // Vaut true si la condition est || et que le code de retour est est différent de zéro (erreur)
        temp2 = (WIFEXITED(status) && (WEXITSTATUS(status) == 0) && st.go_on_condition == GO_ON_IF_SUCCESS);
        // Vaut true si la condition est && et que le code de retour est est égale à zéro (tout)
        //printf("WIFEXITED : %d, WEXITSTATUS: %d, st.goOnCdt : %d (&&:%d, ||:%d), temp1 : %d, temp2 : %d \n", WIFEXITED(status), WEXITSTATUS(status),st.go_on_condition,GO_ON_IF_SUCCESS,GO_ON_IF_FAILURE, temp1, temp2 );
        if (!(temp1 || temp2)) { // Si on ne doit pas continuer, pour une raison ou l'autre
            break;
        }
    }

    return 0;
}
