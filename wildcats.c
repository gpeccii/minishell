/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcats.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 09:28:11 by rbordin           #+#    #+#             */
/*   Updated: 2023/07/06 17:54:33 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int counting_dir(char  *cartella)
{
	int             nb;
	DIR				*dir;
	struct dirent	*test;

	nb = 0;
	dir = opendir(cartella);
	if (!dir)
		return (0);
	while (test != NULL)
	{
		if (ft_strncmp(test->d_name, ".", 1) != 0 && ft_strncmp(test->d_name, "..", 2) != 0)
			nb++;
		test = readdir(dir);
	}
	closedir(dir);
	return (nb);
}

static void	**case_one(struct dirent *test, DIR *dir, char **temp)
{
	int	k;

	k = 0;
	while(test != NULL)
	{
		if (ft_strncmp(test->d_name, ".", 1) != 0 && ft_strncmp(test->d_name, "..", 2) != 0)
		{
			temp[k] = ft_strdup(test->d_name);
			k++;
		}
		test = readdir(dir);
	}
	temp[k] = NULL;
}

static void control(char c, t_args *node)
{
	char	*temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	temp = malloc(sizeof(char) * ft_strlen(node->argument) + 1);
	while (node->argument[i] != '\0')
	{
		if (node->argument[i] == c)
		{
			temp[j] = node->argument[i];
			while (node->argument[i] == c)
				i++;
		}
		else if (node->argument[i] != '\0' && node->argument[i] != c)
		{
			temp[j] = node->argument[i];
			i++;
		}
		j++;
	}
	temp[j] = '\0';
	free(node->argument);
	node->argument = ft_strdup(temp);
}

static int	my_strchr(const char *s, int c)
{
	int				i;
	unsigned char	d;

	i = 0;
	d = c;
	while (s[i] != '\0' && s[i] != d)
		i++;
	if (s[i] == '\0' && d != '\0')
		return (-1);
	return (i);
}

static char **wildone(t_shell *mini, t_args *node)
{
	DIR				*dir;
	struct dirent	*test;
	char			pwd_path[1024];
	char			**temp;
	int				i;
	int				j;
	int				k;
	int				flag;

	getcwd(pwd_path, sizeof(pwd_path));
	printf("aaaaa%saaaa\n", pwd_path);
	dir = opendir(pwd_path);
	temp = malloc(counting_dir(pwd_path) + 1 * sizeof(char *));
	test = readdir(dir);
	control('*', node);
	if (strcmp(node->argument, "*") == 0)
	{
		case_one(test, dir, temp);
		return (temp);
	}
	k = 0;
    while (test != NULL)
	{
		if (test->d_type == DT_UNKNOWN)
			test = readdir(dir);
		else
		{
			i = 0;
			j = 0;
			flag = 0;
			while (node->argument[i] != '\0')
			{
				while(node->argument[i] == test->d_name[j])
				{
					i++;
					j++;
				}
				if (node->argument[i] != test->d_name[j] && node->argument[i] != '*')
					break;
				if (node->argument[i] == '*')
				{
					i++;
					while (node->argument[i] != test->d_name[j] && test->d_name[j] != '\0')
						j++;
					while (node->argument[i] != '\0' && test->d_name[j] != '\0')
					{
						if (node->argument[i] == test->d_name[j] && j != 0)
						{
							i++;
							j++;
						}
						else if (node->argument[i] == '*')
						{
							i++;
							while(node->argument[i] != test->d_name[j] && test->d_name[j] != '\0')
								j++;
						}
						else
						{
							flag = 1;
							break;
						}
					}
					if (flag == 0 && node->argument[i] == '\0')
					{
						temp[k] = ft_strdup(test->d_name);
						k++;
					}
					if (flag = 1)
						break;
				}
				if (node->argument[i] != '*')
					i++;
			}
			test = readdir(dir);
		}
	}
	temp[k] = NULL;
    closedir(dir);
	if (temp[0] == NULL)
		return (NULL);
	return (temp);
}


void    wild(t_shell *mini, t_args **node)
{
	t_args  *cur;
	int     i;
	int		c;
	char	**ret;


	cur = *node;
	ret = NULL;
	while (cur != NULL)
	{
		if (cur->argument != NULL && my_strchr(cur->argument, '*') != -1)
			i = my_strchr(cur->argument, '*');
		if (cur->argument != NULL && my_strchr(cur->argument, '*') != -1 && (checking_quotes_for_operator(cur->argument, '\"', i) == 1 && checking_quotes_for_operator(cur->argument, '\'', i) == 1))
		{
			ret = wildone(mini, cur);
			if (ret != NULL)
			{
				free(cur->argument);
				cur->argument = NULL;
				i = 1;
				cur->argument = ft_strdup(ret[0]);
				while (ret[i] != NULL)
				{
					cur->argument = ft_strjoin_mini(cur->argument, ret[i], FREE, NO_FREE);
					i++;
				}
				free_matrix(ret);
			}
		}
		if (cur->next == NULL)
			return ;
		cur = cur->next;
	}
}

