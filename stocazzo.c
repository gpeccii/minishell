/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stocazzo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcologgi <dcologgi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 13:57:51 by rbordin           #+#    #+#             */
/*   Updated: 2023/06/29 11:35:25 by dcologgi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char **create_matri(t_args *node) {
	char **temp = NULL;
	int size = 0;

	if (node->command != NULL)
		size++;
	if (node->flags != NULL)
		size++;
	if (node->argument != NULL)
		size++;

	temp = malloc((size + 1) * sizeof(char *));
	if (temp == NULL) {
		perror("Failed to allocate memory");
		return NULL;
	}
	
	int index = 0;
	if (node->command != NULL) {
		temp[index] = strdup(node->command);
		index++;
	}
	if (node->flags != NULL) {
		temp[index] = strdup(node->flags);
		index++;
	}
	if (node->argument != NULL) {
		temp[index] = strdup(node->argument);
		index++;

	}

	temp[index] = NULL;
	// for(int i=0; temp[i]; i++)
	// 	printf("matr : %s \n",temp[i]);
	return (temp);
}


int execpipe(t_shell *mini, t_args **node, char **envp) {
    t_args *cur = *node;
    int num_pipes = 0;
    int pid;
    int has_pipe;
    char ***commands = NULL;
    int *fds = NULL;
    int i;

    // Conta il numero di comandi collegati da pipe
    while (cur != NULL && cur->next != NULL && cur->next->redirect[0] == '|') {
        num_pipes++;
        cur = cur->next->next;
    }

    // Alloca memoria per le matrici dei comandi e i file descriptor delle pipe
    commands = malloc((num_pipes + 1) * sizeof(char **));
    fds = malloc(num_pipes * 2 * sizeof(int));

    cur = *node;

    // Crea le pipe e le matrici dei comandi
    for (i = 0; i < num_pipes; i++) {
        pipe(fds + (i * 2));
        commands[i] = create_matri(cur);
        cur = cur->next->next;
    }

    // Ultimo comando senza pipe successiva
    commands[num_pipes] = create_matri(cur);

    // Esegui i comandi collegati da pipe
    for (i = 0; i <= num_pipes; i++) {
        pid = fork();

        if (pid == 0) {
            // Figlio: redireziona l'input e l'output in base alla posizione del comando
            if (i == 0) {
                // Primo comando: redireziona solo l'output
                if (dup2(fds[1], 1) == -1)
                    perror("Failed to redirect output to pipe");
            } else if (i == num_pipes) {
                // Ultimo comando: redireziona solo l'input
                if (dup2(fds[(i - 1) * 2], 0) == -1)
                    perror("Failed to redirect input from pipe");
            } else {
                // Comandi intermedi: redireziona sia l'input che l'output
                if (dup2(fds[(i - 1) * 2], 0) == -1)
                    perror("Failed to redirect input from pipe");
                if (dup2(fds[i * 2 + 1], 1) == -1)
                    perror("Failed to redirect output to pipe");
            }

            // Chiudi tutti i file descriptor delle pipe
            for (int j = 0; j < num_pipes * 2; j++)
                close(fds[j]);
            
        }
    }

    // Chiudi tutti i file descriptor delle pipe nel processo padre
    for (i = 0; i < num_pipes * 2; i++)
        close(fds[i]);

    // Attendere tutti i processi figli
    for (i = 0; i <= num_pipes; i++)
        wait(NULL);

    // Liberare la memoria
    for (i = 0; i <= num_pipes; i++)
        free_matrix(commands[i]);

    free(commands);
    free(fds);
}
