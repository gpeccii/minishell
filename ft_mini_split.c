/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mini_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbordin <rbordin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 12:32:11 by rbordin           #+#    #+#             */
/*   Updated: 2023/06/26 10:29:41 by rbordin          ###   ########.fr       */
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
			if (s[i] == '(')
			{
				z = i + 1;
				while (s[z] != '\0')
				{
					if (s[z] == ')' && (checking_quotes_for_operator(s, '\"', z) && checking_quotes_for_operator(s, '\'', z)))
					{
						mini->flags.brackets = 1;
						//printf("brackets = %d\n", mini->flags.brackets);
						i = z;
						
					}
					z++;
				}
				if (mini->flags.brackets == 0)
				{
					printf("Error: missing final brackets\n");
					mini->exit = 1;
					return ;
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

char	**ft_mini_split(t_shell *mini, char const *s, char c)
{
	unsigned int	i;
	char			**matrix;
	char			**final;
	
	final = NULL;
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
	if ((operator_checker(s, '|') != 0) || (operator_checker(s, '&') != 0))
	{
		mini->flags.operators = 1;
		mini->flags.command_separator = 1;
	}
	if (mini->flags.operators == 1)
	{
		final = creting_matrix_to_populate_nodes(matrix);
		free_matrix(matrix);
	}
	else
	{
		final = new_function(matrix);
		if (final == NULL)
		{
			final = ft_calloc(1, sizeof(char *));
			final[0] = ft_strdup(s);
			final[1] = NULL;
		}
		free_matrix(matrix);
	}
	return (final);
		
}

#include <string.h>
char	**creting_matrix_to_populate_nodes(char **matrix)
{
	int i = 0;
	int k = 0;
	int	z = 1;
	char	**final;
	
	while (matrix[i])
	{
		if ((strcmp(matrix[i], "&&") == 0 ) || (strcmp(matrix[i], "||") == 0 ))
			k++;
		i++;
	}
	if (k == 0)
		return (matrix);
	final = ft_calloc(((2 * k) + 2), sizeof(char *));
	i = 0;
	k = 0;
	while (matrix[i])
	{
		final[k] = ft_calloc(ft_strlen(matrix[i]) + 1, sizeof(char));
		final[k] = ft_memcpy(final[k], ".", 1);
		while (matrix[i] && ((strcmp(matrix[i], "&&") != 0 ) && (strcmp(matrix[i], "||") != 0 )))
		{
			final[k] = ft_strjoin_mini(final[k], matrix[i], NO_FREE, NO_FREE);
			final[k] = ft_strtrim(final[k], ". ");
			i++;
		}
		if (matrix[i] == NULL)
			break;
		if ((strcmp(matrix[i], "&&") == 0 ) || (strcmp(matrix[i], "||") == 0 ))
		{
			k++;
			final[k] = ft_strdup(matrix[i]);
			k++;
		}
		i++;
	}
	//int	j = -1;
	//while (final[++j])
	//	////printf("+++++%s\n", final[j]);
	return(final);
}

int	operator_checker(const char *s, char control)
{
	int	i;

	i = 0;
	////printf("///////s = %s\n", s);
	while (s[i])
	{	
		if (s[i] == control)
		{
			if (s[i + 1] == control && s[i - 1] != '\\')
			{
				if ((checking_quotes_for_operator(s, '\"', i) == 0) || (checking_quotes_for_operator(s, '\'', i) == 0))
					return (0);
				return(1);
			}
		}
		i++;
	}
	return (0);
}

int	checking_quotes_for_operator(const char *s, char control, int pos)
{
	int i;
	int z;
	int flag;

	flag = 0;
	z = 0;
	i = 0;
	while (i <= pos)
	{
		if (flag == 0 && s[i] == control)
			flag = 1;
		else if (flag == 1 && s[i] == control)
			flag = 0;
		i++;
	}
	if (flag == 1)
	{
		while (s[i] != '\0')
		{
			if (s[i] == control)
			{
				flag = 0;
				return (0);
			}
			i++;
		}
	}
	return (1);
}

char	*ft_strjoin_mini(char const *s1, char const *s2, int free1, int free2)
{
	char	*s3;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	j = 0;
	i = 0;
	s3 = (char *)malloc((ft_strlen(s1) + ft_strlen(s2)) + 2 * sizeof(char));
	if (!s3)
		return (NULL);
	while (s1 && s1[i] != '\0')
	{
		((char *)s3)[i] = s1[i];
		i++;
	}
	s3[i] = ' ';
	i++;
	while (s2 && s2[j] != '\0')
		((char *)s3)[i++] = s2[j++];
	((char *)s3)[i] = '\0';
	if (s1 && free1 == FREE)
		free((void *)s1);
	if (s2 && free2 == FREE)
		free((void *)s2);
	return (s3);
}

int	checking_brackets_for_operator(const char *s, int pos)
{
	int i;
	int z;
	int flag;

	flag = 0;
	z = 0;
	i = 0;
	while (i <= pos)
	{
		if (flag == 0 && s[i] == '(')
			flag = 1;
		if (flag == 1 && s[i] == ')')
			flag = 0;
		i++;
	}
	if (flag == 1)
	{
		while (s[i] != '\0')
		{
			if (s[i] == ')')
			{
				flag = 0;
				return (0);
			}
			i++;
		}
		//perror("Minishell");
	}
	return (1);
}

char	**new_function(char **matrix)
{
	int i = 0;
	int k = 0;
	int	z = 1;
	char	**final;
	
	while (matrix[i])
	{
		if (matrix[i][0] == '|' || matrix[i][0] == '<' || matrix[i][0] == '>') 
			k++;
		i++;
	}
	if (k == 0)
		return (NULL);
	final = ft_calloc(((2 * k) + 2), sizeof(char *));
	i = 0;
	k = 0;
	while (matrix[i])
	{
		final[k] = ft_calloc(ft_strlen(matrix[i]) + 1, sizeof(char));
		final[k] = ft_memcpy(final[k], ".", 1);
		while (matrix[i] && (matrix[i][0] != '|' || matrix[i][0] != '<' || matrix[i][0] != '>'))
		{
			final[k] = ft_strjoin_mini(final[k], matrix[i], NO_FREE, NO_FREE);
			final[k] = ft_strtrim(final[k], ". ");
			i++;
		}
		if (matrix[i] == NULL)
			break;
		if (matrix[i][0] == '|' || matrix[i][0] == '<' || matrix[i][0] == '>')
		{
			k++;
			final[k] = ft_strdup(matrix[i]);
			k++;
		}
		i++;
	}
	//int	j = -1;
	//while (final[++j])
	//	////printf("+++++%s\n", final[j]);
	return(final);
}