/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 16:49:53 by tpiras            #+#    #+#             */
/*   Updated: 2023/11/29 14:54:09 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

void	gest_signal(void)
{
	signal(SIGINT, handlectrlc);
	signal(SIGTERM, handlectrlc);
	signal(SIGQUIT, SIG_IGN);
}

void	reset(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_point = 0;
		rl_end = 0;
		rl_redisplay();
	}
}

void	handlectrl(t_shell *mini, char **envp)
{
	(void)envp;
	if (!mini->input)
	{
		clear_mini(mini, 1);
		write(1, "exit\n", 5);
		exit(0);
	}
}

void	handlectrlc(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signal == SIGTERM)
		exit (1);
}

void	sig_ign(int signal)
{
	printf("\n");
	(void)signal;
}
