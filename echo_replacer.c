/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_replacer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 12:08:51 by riccardobor       #+#    #+#             */
/*   Updated: 2023/07/06 17:23:10 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// norma ok
extern int	g_exit_status;

static char	*echo_expanding_d(t_shell *mini, char *s)
{
	char	*en;

	en = getenv(s);
	if (s[0] == '?' && s[1] == '\0')
	{
		if (mini->flag_status != 0)
		{
			en = ft_itoa(mini->flag_status);
			mini->flag_status = 0;
		}
		else
			en = ft_itoa(WEXITSTATUS(g_exit_status));
	}
	if (en == NULL)
		return (s);
	return (en);
}

static int	ft_conta(char *str, char c)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			counter++;
		i++;
	}
	return (counter);
}

static char	*ft_strongest(t_args *node, int i, int j, char *tmp)
{
	while (node->argument[i] != '\0')
	{
		if (node->argument[i] == '$')
		{
			tmp[j] = 6;
			i++;
			j++;
			while (node->argument[i] != ' ' && node->argument[i] != '\0')
				tmp[j++] = node->argument[i++];
			if (node->argument[i] != '\0')
			{
				tmp[j] = 6;
				j++;
			}
			if (node->argument[i] == '\0')
			{
				tmp[j] = '\0';
				return (tmp);
			}
		}
		tmp[j++] = node->argument[i++];
	}
	tmp[j] = '\0';
	return (tmp);
}

static char	*ft_quite_strong(t_args *node)
{
	int		i;
	int		j;
	char	*tmp;
	int		counter;

	i = 0;
	j = 0;
	counter = ft_conta(node->argument, '$');
	tmp = malloc(sizeof(char) * (ft_strlen(node->argument) + (2 * counter)));
	tmp = ft_strongest(node, i, j, tmp);
	return (tmp);
}

void	echo_replacer(t_shell *mini, t_args *node)
{
	int		i;
	char	*en;
	char	**matrix;
	char	*str;

	i = 0;
	matrix = NULL;
	en = ft_strdup("");
	if (node->argument != NULL && (ft_strchr(node->argument, '$') != 0
			|| node->argument[0] == '$'))
	{
		str = ft_quite_strong(node);
		matrix = ft_split(str, 6);
		free(str);
		while (matrix[i] != NULL)
		{
			matrix[i] = echo_expanding_d(mini, matrix[i]);
			en = ft_strjoin(en, matrix[i], NO_FREE, NO_FREE);
			i++;
		}
		free(node->argument);
		node->argument = ft_strdup(en);
	}
	free(en);
}
