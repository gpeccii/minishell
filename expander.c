/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 12:08:51 by riccardobor       #+#    #+#             */
/*   Updated: 2023/07/06 17:11:20 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

void	saver(t_shell *mini, t_args *node)
{
	t_args	*cur;
	int		i;

	i = 0;
	cur = node;
	mini->dollar = NULL;
	while (cur->argument && cur->argument[i] != '$')
		i++;
	if (cur->argument[i] == '$')
	{
		if (checking_quotes_for_operator(cur->argument, '\"', i))
			get_dollars(mini, cur->argument);
	}
	return ;
}

void	get_dollars(t_shell *mini, char *s)
{
	int		i;
	int		k;
	char	c;

	mini->dollar = ft_split(s, '=');
	mini->dollar[1] = ft_strtrim(mini->dollar[1], "\"");
}

void	replacer(t_shell *mini, t_args *node)
{
	int		i;
	char	*en;

	i = 0;
	while (node != NULL)
	{
		if (node->argument != NULL)
		{
			en = expanding_d(mini, node->argument);
			if (en != NULL)
			{
				node->argument = ft_strdup(en);
				break ;
			}
		}
		node = node->next;
	}
}

char	*expanding_d(t_shell *mini, char *s)
{
	char	*d;
	char	*en;

	d = ft_strtrim(s, "$");
	en = getenv(d);
	if (d[0] == '?' && d[1] == '\0')
	{
		if (mini->flag_status != 0)
		{
			en = ft_itoa(mini->flag_status);
			mini->flag_status = 0;
		}
		else
			en = ft_itoa(WEXITSTATUS(g_exit_status));
	}
	if (!en)
		return (NULL);
	free(d);
	free(s);
	return (en);
}
