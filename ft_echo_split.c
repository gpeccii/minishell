/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbordin <rbordin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 12:32:11 by rbordin           #+#    #+#             */
/*   Updated: 2023/06/30 11:55:07 by rbordin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static unsigned int	ft_counter(char const *s, char c)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (count);
}

static int	ft_strcheck(char const *s, char c)
{
	int			i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static void	ft_cicle(t_shell *mini, char const *s, char **str, char c, unsigned int count)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	k;
	int	z;

	i = 0;
	k = 0;
	z = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		j = i;
		while (s[i] != c && s[i] != '\0')
		{
			if (s[i] == '\'')
			{
				z = i + 1;
				while (s[z] != '\'' && s[z])
					z++;
				if (s[z] == '\'')
				{
					mini->flags.quote_flag = 1;
					i = z;
				}
				else if (mini->flags.quote_flag == 0)
				{
					printf("Error: missing final quote\n");
					mini->exit = 1;
				}
			}
			else if (s[i] == '\"')
			{
				z = i + 1;
				while (s[z] != '\"' && s[z])
					z++;
				if (s[z] == '\"')
				{
					mini->flags.quote_flag = 1;
					i = z;
				}
				else if (mini->flags.quote_flag == 0)
				{
					printf("Error: missing final quote\n");
					mini->exit = 1;
				}
			}
			i++;
		}
		if (k < count)
		{
			if (mini->flags.quote_flag == 1)
				str[k] = ft_substr(s, j, (size_t)(z + 1 - j));
			else if (mini->flags.quote_flag == 1)
				str[k] = ft_substr(s, j, (size_t)(z + 1 - j));
			else
				str[k] = ft_substr(s, j, (size_t)(i - j));
			k++;
			mini->flags.quote_flag = 0;
			//printf("str[%d] = %s\n", k, str[k]);
		}
	}
	str[k] = NULL;
	return ;
}

char	**ft_echo_split(t_shell *mini, char const *s, char c)
{
	unsigned int	i;
	char			**matrix;

	if (!s)
		return (NULL);
	i = ft_counter(s, c);
	matrix = (char **)malloc((i + 1) * sizeof (char *));
	if (!matrix)
		return (NULL);
	if (i == 0)
		matrix[0] = NULL;
	else if (i == 1 && !ft_strcheck(s, c))
	{
		matrix[0] = ft_substr(s, 0, ft_strlen(s));
		matrix[1] = NULL;
	}
	else
		ft_cicle(mini, s, matrix, c, i);
	return (matrix);
}
