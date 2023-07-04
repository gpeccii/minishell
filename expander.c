/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbordin <rbordin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 12:08:51 by riccardobor       #+#    #+#             */
/*   Updated: 2023/06/26 14:44:54 by rbordin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

void	saver(t_shell *mini, t_args *node)
{
	t_args *cur;
	int	i;

	i = 0;
	cur = node;
	mini->dollar = NULL;
	while (cur->argument && cur->argument[i] != '$')
		i++;
	//printf("i = %d\n", i);
	//printf("arg = %s\n", cur->argument);
	//printf("arg = %s\n", node->argument);

	if (cur->argument[i] == '$')
	{
		//printf("saver\n");
		if (checking_quotes_for_operator(cur->argument, '\"', i))
			get_dollars(mini, cur->argument);
	}
	//printf("$flag = %d\n", mini->dollar_flag);
	// //printf("dollar[0] = %s\n", mini->dollar[0]);
	// //printf("dollar[1] = %s\n", mini->dollar[1]);
	// replacer(mini, node);
	return ;
}

void	get_dollars(t_shell *mini, char *s)
{
	int		i;
	int		k;
	char	c;

	//printf("get_dollars\n");
	mini->dollar = ft_split(s, '=');
	mini->dollar[1] = ft_strtrim(mini->dollar[1], "\"");
	// int j = -1;
	// while (mini->dollar[++j])
	// 	//printf("dollar = %s\n", mini->dollar[j]);
}

void	replacer(t_shell *mini, t_args *node)
{
	int		i;
	char	*en;
	//printf("replacer\n");
	i = 0;
	while (node != NULL)
	{
		//printf("cur arg = %s\n", node->argument);
		if (node->argument != NULL)
		{
			if ((en = expanding_d(mini, node->argument)) != NULL)				
			{
				
				//printf("env = %s\n", en);	
				node->argument = ft_strdup(en);
				break;
			}
			// else
			// 	node->argument = expanding_dollar_own_variables(mini, node->argument);
				
		}
		node = node->next;
	}
}

char	*expanding_d(t_shell *mini, char *s)
{
	char	*d;
	char	*en;
	
	d = ft_strtrim(s, "$");
	// //printf("----------------------d = %s\n", d);
	en = getenv(d);
	if (d[0] == '?' && d[1] == '\0')
	{
		//printf("----%d\n", mini->flag_status);
		if (mini->flag_status != 0)
		{
			en = ft_itoa(mini->flag_status);
			mini->flag_status = 0;
		}
		else
			en = ft_itoa(WEXITSTATUS(g_exit_status));
	}
	//printf("env = %s\n", en);	
	if (!en)
		return (NULL);

	free(d);
	free(s);
	return (en);
}