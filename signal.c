/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcologgi <dcologgi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:12:59 by dcologgi          #+#    #+#             */
/*   Updated: 2023/06/29 11:13:15 by dcologgi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handlectrl(t_shell *mini, char **envp)
{
    char input[256];

    if (!mini->input || ft_strncmp(mini->input, "exit", 4) == 0)
    {
        write(1, "exit\n", 5);
        exit(0);
    }

}

void handlectrlc(int signal)
{
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}
