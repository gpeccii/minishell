/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dick.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbordin <rbordin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 16:46:47 by rbordin           #+#    #+#             */
/*   Updated: 2023/06/30 15:21:55 by rbordin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

static int	ft_argssize(t_args *lst)
{
	int	i;

	i = 0;
	if (!lst)
		return (0);
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	//printf("len = %d\n", i);
	return (i);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	mini;
	t_args	*test;
	int		i;
	char	*paths;
	
	(void)argc;
	(void)argv;
	mini.main_path = "Minishell$ ";
	mini.syntax = "$";
	mini.exit = 0;
	i = 0;
	signal(SIGINT, handlectrlc);
	signal(SIGQUIT, SIG_IGN);
	test = NULL;
	mini.list = test;
	while (envp[i])
		i++;
	mini.envp = malloc((i + 1) * sizeof(char *));
	i = 0;
	while (envp[i])
	{
		mini.envp[i] = ft_strdup(envp[i]);
		i++;
	}
	mini.envp[i] = NULL;
	mini.flag_status = 0;
	mini.home = getcwd(mini.home, sizeof(mini.home));
	while (mini.exit == 0)
	{
		init_flags(&mini);
		//if (mini.main_path == NULL)
		//	mini.input = readline("$ ");
		mini.input = readline(mini.main_path);
		handlectrl(&mini, envp);
		add_history(mini.input);
		// if (ft_strlen(mini.input) > 0)
		// {
			
			//editing(&mini, mini.input);
			////printf("final = %s\n", mini.input);
		if (start(&mini) == 1)
		{
			insert_last_with_delimiter(&mini, &mini.list, envp, ' ');
			if (mini.exit == 0)
			{
				wild(&mini, &mini.list);
				mini.flags.operators = 0;
				print_list(mini.list);

				//if (ft_argssize(mini.list) > 1)
				//printf("SONO QUIIIII\n");
				replacer(&mini, mini.list);
				execpipe(&mini, mini.list);
				//else
				//	single(&mini, mini.list, envp);
			}
			clear_list(&mini.list);
		}
		mini.exit = 0;
	}
	return (0);
}

void	init_flags(t_shell *mini)
{
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
static int *update(int	*spot, int z)
{
	int	i = 0;
	int	*test;
	
	test = ft_calloc(z, sizeof(int));
	while(spot[i++])
		test[i] = spot[i] + 1;
	free(spot);
	return(test);
}

void	print_error(char *error)
{
	printf("Minishell: %s\n", error);
	return ;
}
