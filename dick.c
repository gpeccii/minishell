/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dick.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 16:46:47 by rbordin           #+#    #+#             */
/*   Updated: 2023/07/06 17:26:45 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

static void	cloning_envp(t_shell *mini, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	mini->envp = malloc((i + 1) * sizeof(char *));
	i = 0;
	while (envp[i])
	{
		mini->envp[i] = ft_strdup(envp[i]);
		i++;
	}
	mini->envp[i] = NULL;
}

static void	init_first_stage(t_shell *mini, char **envp)
{
	t_args	*test;

	test = NULL;
	mini->main_path = "Minishell$ ";
	mini->syntax = "$";
	mini->exit = 0;
	mini->list = test;
	cloning_envp(mini, envp);
	mini->home = getcwd(mini->home, sizeof(mini->home));
}

static void	second_stage(t_shell *mini)
{
	//print_list(mini->list);
	wild(mini, &mini->list);
	mini->flags.operators = 0;
	replacer(mini, mini->list);
	execpipe(mini, mini->list);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	mini;

	signal(SIGINT, handlectrlc);
	signal(SIGQUIT, SIG_IGN);
	init_first_stage(&mini, envp);
	while (mini.exit == 0)
	{
		init_flags(&mini);
		mini.input = readline(mini.main_path);
		handlectrl(&mini, envp);
		if (ft_strlen(mini.input) != 0)
			add_history(mini.input);
		if (start(&mini) == 1)
		{
			insert_last_with_delimiter(&mini, &mini.list, ' ');
			if (mini.exit == 0)
				second_stage(&mini);
			clear_list(&mini.list);
		}
		mini.exit = 0;
	}
	return (0);
}

void	init_flags(t_shell *mini)
{
	mini->flag_status = 0;
	mini->flags.command_separator = 0;
	mini->flags.operators = 0;
	mini->flags.major = 0;
	mini->flags.minor = 0;
	mini->flags.redirect_input = 0;
	mini->flags.redirect_output = 0;
	mini->flags.brackets = 0;
	mini->flags.pipes = 0;
	mini->flags.quote_flag = 0;
	mini->dollar_flag = 0;
	mini->command_presence = 0;
}
