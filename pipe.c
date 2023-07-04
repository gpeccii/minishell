/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbordin <rbordin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 13:57:51 by rbordin           #+#    #+#             */
/*   Updated: 2023/06/30 16:47:20 by rbordin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

static char **create_matri(t_args *node)
{
    char **temp = NULL;
    int size;
    int index;
    char **arguments = NULL;
    int j = -1;
    size = 0;
    if (node->command != NULL)
        size++;
    if (node->flags != NULL)
        size++;
    if (node->argument != NULL)
    {
        arguments = ft_split(node->argument, ' ');
        while (arguments[++j])
            size++;
    }
    temp = malloc((size + 1) * sizeof(char *));
    if (temp == NULL)
    {
        perror("Failed to allocate memory");
        return NULL;
    }
    index = 0;
    if (node->command != NULL)
        temp[index++] = strdup(node->command);
    if (node->flags != NULL)
        temp[index++] = strdup(node->flags);
    if (node->argument != NULL)
    {
        j = 0;
        while (arguments[j])
        {
            temp[index++] = strdup(arguments[j++]);
            free(arguments[j - 1]);
        }
        free(arguments);
    }
    temp[index] = NULL;
    return (temp);
}


static char **redirect_input_no_pipes(t_shell *mini, t_args *node, char ***commands, char **envp)
{
    int temp_fd;

    temp_fd = 1;
    if (node->infile != NULL)
    {
        //printf("111\n");
        // if (node->redirection_quantity == 1)
        //     temp_fd = open(node->infile, O_RDONLY);
        if (node->redirection_quantity == 2)
        {
            screening_terminal(mini, node, temp_fd);
            if (strcmp(node->command, "/usr/bin/cat") == 0)
            {
                temp_fd = open(".res", O_RDONLY);
                if (commands[0][1] == NULL)
                    commands[0][1] = ft_strdup(".res");
                else if (commands[0][2] == NULL)
                    commands[0][2] = ft_strdup(".res");
            }
        }
        dup2(1, temp_fd);
        close(temp_fd);
        //printf("33333\n");
    }
}

void execpipe(t_shell *mini, t_args *node)
{
    t_args *cur = node;
    int pid;
    int temp_fd;
    int temp_fd_out;
    int back_up;
    char ***commands = NULL;
    t_pipex *pipes;

    pipes = malloc(sizeof(t_pipex));

    init_pipexxx(pipes, mini, cur);
    // Alloca memoria per le matrici dei comandi e i file descriptor delle pipe
    commands = malloc((pipes->num_pipes + 1) * sizeof(char **));
    


    cur = node;
    pipes->i = 0;
    // Crea le pipe e le matrici dei comandi
    while (pipes->i < pipes->num_pipes)
    {
        pipe(pipes->fds + (pipes->i * 2));
        commands[pipes->i] = create_matri(cur);
        cur = cur->next->next;
        pipes->i++;
    }

    // Ultimo comando senza pipe successiva
    commands[pipes->num_pipes] = create_matri(cur);
    cur = node;

    // Esegui i comandi collegati da pipe
    pipes->i = 0;

    while (pipes->i <= pipes->num_pipes)
    {
        if(check_builtin_presence(mini,cur->command)==0)
            builtin_pipe(mini, pipes, cur);
        else if(check_builtin_presence(mini,cur->command)>0)
        {
            pid = fork();
            //printf("111111111111\n");

            if (pid == 0)
            {
               // printf("\ngcook debugging\n%s\n", commands[pipes->i][0]);
                //printf("porcodio il comando  é: %s\n", commands[i][0]);
                if (pipes->i == 0 && pipes->num_pipes == 0 && cur->infile != NULL && cur->redirection_quantity == 2)
                    redirect_input_no_pipes(mini, cur, commands, mini->envp);
                if (pipes->i == 0 && pipes->num_pipes == 0 && cur->infile != NULL && cur->redirection_quantity == 1)
                {
                    temp_fd = open(cur->infile, O_RDONLY, 0777);
                    if (dup2(temp_fd, 0) == -1)
                        perror("Failed to redirect output to pipe");
                    close(temp_fd);
                }
                if (pipes->i == 0 && pipes->num_pipes == 0 && cur->outfile != NULL)
                {
                    if (cur->redirection_quantity == 1)
                        temp_fd_out = open(cur->outfile, O_TRUNC | O_CREAT | O_RDWR, 0777);
                    else if (cur->redirection_quantity == 2)
                        temp_fd_out = open(cur->outfile, O_APPEND | O_CREAT | O_RDWR, 0777);
                    dup2(temp_fd_out, STDOUT_FILENO);
                }
                // Figlio: redireziona l'input e l'output in base alla posizione del comando
                else if (pipes->i == 0 && pipes->i != pipes->num_pipes)
                {
                    //printf("culo\n%s, %s\nculo\n", cur->infile, cur->outfile);
                    if (cur->infile != NULL)
                    {
                        if (cur->redirection_quantity == 1)
                        {
                            //printf("dioporco\n");
                            temp_fd = open(cur->infile, O_RDONLY, 0777);
                            dup2(temp_fd, 0);
                        }
                        else if (cur->redirection_quantity == 2)
                        {
                            screening_terminal(mini, cur, temp_fd);
                            //printf("cur %d\n", strcmp(cur->command, "/usr/bin/cat"));
                            //write(1, "TERMOSTATO", 10);
                            if (strcmp(cur->command, "/usr/bin/cat") == 0)
                            {
                                
                                //temp_fd = open("res", O_RDONLY);
                                commands[0] = create_matri(cur);
                                //for (int j = 0; commands[0][j]; j++)
                            }
                        }
                    }
                    if (cur->outfile != NULL)
                    {
                        //printf("CAZZO%d-----%d\n", i, pipes->num_pipes);
                        if (cur->redirection_quantity == 1)
                            temp_fd_out = open(cur->outfile, O_TRUNC | O_CREAT | O_RDWR, 0777);
                        else if (cur->redirection_quantity == 2)
                            temp_fd_out = open(cur->outfile, O_APPEND | O_CREAT | O_RDWR, 0777);
                        dup2(temp_fd_out, 1);
                    }
                    // Primo comando: redireziona solo l'output
                    if (cur->outfile == NULL)
                    {
                        if (dup2(pipes->fds[1], 1) == -1)
                            perror("Failed to redirect output to pipe");
                        //("111\n");
                    }
                }
                else if (pipes->i == pipes->num_pipes && pipes->i != 0)
                {
                    //printf("culo\n");
                    // printf("culo\n%s, %s\nculo\n", cur->infile, cur->outfile);
                    if (cur->infile != NULL)
                    {
                        if (cur->redirection_quantity == 1)
                        {
                            temp_fd = open(cur->infile, O_RDONLY, 0777);
                            dup2(temp_fd, 0);
                        }
                        else if (cur->redirection_quantity == 2)
                        {
                            screening_terminal(mini, cur, temp_fd);
                            //printf("current %d\n", strcmp(cur->command, "/usr/bin/cat"));
                            if (strcmp(cur->command, "/usr/bin/cat") == 0)
                            {
                                //temp_fd = open("res", O_RDONLY);
                                commands[pipes->i] = create_matri(cur);
                                //for (int j = 0; commands[0][j]; j++)
                                //printf("mtr : %s", commands[0][j]);
                            }
                        }
                    }
                    else if (cur->infile == NULL)
                    {
                        if (dup2(pipes->fds[(pipes->i - 1) * 2], 0) == -1)
                            perror("Failed to redirect input from pipe");
                    }
                    if (cur->outfile != NULL)
                    {
                        if (cur->redirection_quantity == 1)
                            temp_fd_out = open(cur->outfile, O_TRUNC | O_CREAT | O_RDWR, 0777);
                        else if (cur->redirection_quantity == 2)
                            temp_fd_out = open(cur->outfile, O_APPEND | O_CREAT | O_RDWR, 0777);
                        dup2(temp_fd_out, 1);
                        //close(temp_fd_out);
                        // printf("CAZZO%d-----%d\n", pipes->i, pipes->num_pipes);
                    }
                    // Ultimo comando: redireziona solo l'input
                }
                else if (pipes->i != 0 && pipes->i != pipes->num_pipes)
                {
                    // Comandi intermedi: redireziona sia l'input che l'output
                    if (cur->infile != NULL)
                    {
                        if (cur->redirection_quantity == 1)
                        {
                            temp_fd = open(cur->infile, O_RDONLY, 0777);
                            dup2(temp_fd, 0);
                        }
                        else if (cur->redirection_quantity == 2)
                        {
                            screening_terminal(mini, cur, temp_fd);
                            //printf("current %d\n", strcmp(cur->command, "/usr/bin/cat"));
                            if (strcmp(cur->command, "/usr/bin/cat") == 0)
                            {
                                //temp_fd = open("res", O_RDONLY);
                                commands[pipes->i] = create_matri(cur);
                                //for (int j = 0; commands[0][j]; j++)
                                //printf("mtr : %s", commands[0][j]);
                            }
                        }
                    }
                    else if (cur->infile == NULL)
                    {
                        if (dup2(pipes->fds[(pipes->i - 1) * 2], 0) == -1)
                            perror("Failed to redirect input from pipe");
                    }
                    if (cur->outfile != NULL)
                    {
                        if (cur->redirection_quantity == 1)
                            temp_fd_out = open(cur->outfile, O_TRUNC | O_CREAT | O_RDWR, 0777);
                        else if (cur->redirection_quantity == 2)
                            temp_fd_out = open(cur->outfile, O_APPEND | O_CREAT | O_RDWR, 0777);
                        dup2(temp_fd_out, 1);
                        //printf("CAZZO%d-----%d\n", pipes->i, pipes->num_pipes);
                    }
                    else if (cur->outfile == NULL)
                    {
                        if (dup2(pipes->fds[pipes->i * 2 + 1], 1) == -1)
                            perror("Failed to redirect output to pipe");
                    }
                }

                // Chiudi tutti i file descriptor delle pipe
                for (int j = 0; j < pipes->num_pipes * 2; j++)
                    close(pipes->fds[j]);

                execve(commands[pipes->i][0], commands[pipes->i], mini->envp);
            }
        }
        if (pipes->i < pipes->num_pipes)
            cur = cur->next->next;
        pipes->i++;
    }

    // Chiudi tutti i file descriptor delle pipe nel processo padre
    pipes-> i = 0;
    while (pipes->i < pipes->num_pipes * 2)
    {
        close(pipes->fds[pipes->i]);
        pipes->i++;
    }
    if (temp_fd)
        close(temp_fd);
    if (temp_fd_out)
        close(temp_fd_out);
    // Attendere tutti i processi figli
    pipes->i = 0;
    while (pipes->i <= pipes->num_pipes)
    {
        waitpid(-1, &g_exit_status, 0);
        pipes->i++;
    }
    // Liberare la memoria
    pipes->i = 0;
    while (pipes->i <= pipes->num_pipes)
    {
        free_matrix(commands[pipes->i]);
        pipes->i++;
    }
    free(commands);
    free(pipes->fds);
    if (cur->next != NULL && ft_strncmp(cur->next->redirect, "||", 2) == 0 && WEXITSTATUS(g_exit_status) == 1)
        execpipe(mini, cur->next->next);
    if (cur->next != NULL && ft_strncmp(cur->next->redirect, "&&", 2) == 0)
        execpipe(mini, cur->next->next);
        
}
// un processo andato a buon fine ha exit status = 0.
void screening_terminal(t_shell *mini, t_args *node, int temp_fd)
{
    char *buffer;
    char *res;

    res = ft_strdup("");
    while (1)
    {
        buffer = get_next_line(0);
        //printf("strncmp %d\n", ft_strncmp(buffer, node->infile, ft_strlen(node->infile)));
        if (ft_strncmp(buffer, node->infile, ft_strlen(node->infile)) == 0)
            break;
        res = ft_strjoin(res, buffer, FREE, NO_FREE);
    }
    free(buffer);
    free(node->infile);
    node->infile = NULL;
    if (strcmp(node->command, "/usr/bin/cat") == 0)
    {
        temp_fd = open(".dick", O_TRUNC | O_CREAT | O_RDWR);
        ft_putstr_fd(res, temp_fd);
        node->argument = ft_strdup(".dick");
        close(temp_fd);
    }
    else
        node->argument = ft_strdup(res);
    free(res);
}
