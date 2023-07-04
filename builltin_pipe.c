
#include "minishell.h"

void init_pipexxx(t_pipex *pipes, t_shell *mini, t_args *cur)
{
    pipes->num_pipes = 0;
    while (cur != NULL && cur->next != NULL && cur->next->redirect[0] == '|' && cur->next->redirect[1] != '|')
    {
        pipes->num_pipes++;
        cur = cur->next->next;
    }
	pipes->i = 0;
    pipes->fds = malloc(pipes->num_pipes * 2 * sizeof(int));
}

void builtin_pipe(t_shell *mini, t_pipex *pipes, t_args *cur)
{
    int original_std_out;
    int original_std_in;
    int temp_fd;
    int temp_fd_out;

    original_std_out = dup(1);
    original_std_in = dup(0);
            //printf("porcodio il comando  é: %s\n", commands[i][0]);
            // if (pipes->i == 0 && pipes->num_pipes == 0 && cur->infile != NULL && cur->redirection_quantity == 2)
            //     redirect_input_no_pipes(mini, cur, commands, mini->envp);
            if (pipes->i == 0 && pipes->num_pipes == 0 && cur->infile != NULL && cur->redirection_quantity == 1)
            {
                temp_fd = open(cur->infile, O_RDONLY);
                if (dup2(temp_fd, 0) == -1)
                    perror("Failed to redirect output to pipe");
            }
            if (pipes->i == 0 && pipes->num_pipes == 0 && cur->outfile != NULL)
            {
                if (cur->redirection_quantity == 1)
                    temp_fd_out = open(cur->outfile, O_TRUNC | O_CREAT | O_RDWR, 0777);
                else if (cur->redirection_quantity == 2)
                    temp_fd_out = open(cur->outfile, O_APPEND | O_CREAT | O_RDWR, 0777);
                dup2(temp_fd_out, 1);

            }
            // else if (pipes->i == pipes->num_pipes && pipes->i == 0 && cur->outfile != NULL)
            // {
            //     if (cur->redirection_quantity == 1)
            //         temp_fd = open(cur->outfile, O_TRUNC | O_CREAT | O_RDWR, 0777);
            //     else if (cur->redirection_quantity == 2)
            //         temp_fd = open(cur->outfile, O_APPEND | O_CREAT | O_RDWR, 0777);
            //     dup2(temp_fd, 1);
            // }
            // Figlio: redireziona l'input e l'output in base alla posizione del comando
            else if (pipes->i == 0 && pipes->i != pipes->num_pipes)
            {
                if (cur->infile != NULL)
                {
                    if (cur->redirection_quantity == 1)
                    {
                        //printf("dioporco\n");
                        temp_fd = open(cur->infile, O_RDONLY);
                        dup2(pipes->fds[1], 1);
                        // dup2(, 0);
                    }
                    else if (cur->redirection_quantity == 2)
                    {
                        screening_terminal(mini, cur, temp_fd);
                        //printf("cur %d\n", strcmp(cur->command, "/usr/bin/cat"));
                        //write(1, "TERMOSTATO", 10);
                        
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
                if (cur->infile == NULL && cur->outfile == NULL)
                {                    
                    if (dup2(pipes->fds[(pipes->i) * 2 + 1], 1) == -1)
                        perror("Failed to redirect output to pipe");
                    //("111\n");
                }
            }
            else if (pipes->i == pipes->num_pipes && pipes->i != 0)
            {
                if (cur->infile != NULL)
                {
                    if (cur->redirection_quantity == 1)
                        temp_fd = open(cur->infile, O_RDONLY);
                    else if (cur->redirection_quantity == 2)
                    {
                        screening_terminal(mini, cur, temp_fd);
                        //printf("current %d\n", strcmp(cur->command, "/usr/bin/cat"));
                  
                    }
                }
                if (cur->outfile != NULL)
                {
                    //printf("CAZZO%d-----%d\n", pipes->i, pipes->num_pipes);
                    if (cur->redirection_quantity == 1)
                        temp_fd_out = open(cur->outfile, O_TRUNC | O_CREAT | O_RDWR, 0777);
                    else if (cur->redirection_quantity == 2)
                        temp_fd_out = open(cur->outfile, O_APPEND | O_CREAT | O_RDWR, 0777);
                    dup2(temp_fd_out, STDOUT_FILENO);
                }
                else
                {
                    
                }
                // Ultimo comando: redireziona solo l'input
                if (dup2(pipes->fds[(pipes->i - 1) * 2], 0) == -1)
                    perror("Failed to redirect input from pipe");
            }
            else if (pipes->i != 0 && pipes->i != pipes->num_pipes)
            {
                // Comandi intermedi: redireziona sia l'input che l'output
                if (dup2(pipes->fds[(pipes->i - 1) * 2], 0) == -1)
                    perror("Failed to redirect input from pipe");
                if (dup2(pipes->fds[pipes->i * 2 + 1], 1) == -1)
                    perror("Failed to redirect output to pipe");
            }

            // Chiudi tutti i file descriptor delle pipe
            // for (int j = 0; j < pipes->num_pipes * 2; j++)
            //     close(pipes->fds[j]);
            
            builtin_exec(mini, mini->envp, cur,cur->command);
            if (temp_fd)
            {
                dup2(original_std_in, 0);
                close(temp_fd);
                close(original_std_in);
            
            }
            if (temp_fd_out)
            {
                dup2(original_std_out, 1);
                close(temp_fd_out);
                close(original_std_out);
            }
           
            
}
      
    