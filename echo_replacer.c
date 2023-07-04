/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riccardobordin <riccardobordin@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 12:08:51 by riccardobor       #+#    #+#             */
/*   Updated: 2023/07/02 15:58:03 by riccardobor      ###   ########.fr       */
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

static int  ft_conta(char *str, char c)
{
    int i;
    int counter;

    i = 0;
    counter = 0;
    while(str[i] != '\0')
    {
        if (str[i] == c)
            counter++;
        i++;
    }
    return (counter);
}

static char *ft_potente(t_args *node)
{
    int i;
    int j;
    char *tmp;
    int counter;

    i = 0;
    j = 0;
    counter = ft_conta(node->argument, '$');
    tmp = malloc(sizeof(char) * (ft_strlen(node->argument) + (2 * counter)));
    while(node->argument[i] != '\0')
    {
        if (node->argument[i] == '$')
        {
            tmp[j] = 6;
            i++;
            j++;
            while(node->argument[i] != ' ' && node->argument[i] != '\0')
            {
                tmp[j] = node->argument[i];
                i++;
                j++;
            }
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
        tmp[j] = node->argument[i];
        i++;
        j++;
    }
    tmp[j] = '\0';
    return (tmp);
}

void	echo_replacer(t_shell *mini, t_args *node)
{
	int		i;
	char	*en;
	char	**matrix;
    char    *str;

	i = 0;
	matrix = NULL;
	en = ft_strdup("");
	if (node->argument != NULL && (ft_strchr(node->argument, '$') != 0 || node->argument[0] == '$'))
	{
        str = ft_potente(node);
		matrix = ft_split(str, 6);
        free(str);
		while (matrix[i] != NULL)
		{
			matrix[i] = echo_expanding_d(mini, matrix[i]);
			en = ft_strjoin(en, matrix[i], NO_FREE, NO_FREE);
            i++;
		}
        //free_matrix(matrix);
        free(node->argument);
		node->argument = ft_strdup(en);
        //printf("[[[[%ld\n", ft_strlen(node->argument));
	}
    free(en);
}
