/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcats.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbordin <rbordin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 09:28:11 by rbordin           #+#    #+#             */
/*   Updated: 2023/06/30 12:02:35 by rbordin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (cur->argument != NULL)
		{
			i = 0;
			//printf("%s\n", cur->argument);
			while (cur->argument[i])
			{
				if (cur->argument[i] == '*' && (checking_quotes_for_operator(cur->argument, '\"', i) == 1 && checking_quotes_for_operator(cur->argument, '\'', i) == 1 ))
				{
					c = counting_cards(cur->argument);
					ret = cards(cur->argument, i, c);
					free(cur->argument);
					i = 1;
					cur->argument = ft_strdup(ret[0]);
					while (ret[i] != NULL)
					{
						cur->argument = ft_strjoin_mini(cur->argument, ret[i], FREE, NO_FREE);
						i++;
					}
					free_matrix(ret);
					break;
				}
				i++;
			}
		}
		if (cur->next == NULL)
			return ;
		cur = cur->next;
		
	}
}

int	counting_cards(char *argument)
{
	int	k;
	int	total;
	
	k = 0;
	total = 0;
	while (argument[k])
	{
		if (argument[k] == '.')
			total++;
		k++;
	}
	return (total);
}


char	**cards(char *argument, int i, int c)
{
	DIR				*dir;
	char			*cartella;
	struct dirent	*test;
	char            **temp;
	int				k;
	//printf("cards\n");
	int				flag = 0;

	if (argument[i + 1] == '.')
		flag = 1;
	if (argument[i + 2] == '*')
		flag = 2;
	if (c > 1)
		flag = 3;
	if (c > 1 && argument[ft_strlen(argument) - 1] != '*')
		flag = 4;
	k = 0;
	cartella = getenv("PWD");
	//printf("%s\n", cartella);
	dir = opendir(cartella);
	temp = ft_calloc(reading_directory(cartella) + 1, sizeof(char *));
	test = readdir(dir);
	
	while ((test != NULL))
	{
				//printf("flag = %d\n", flag);
		////printf("test = %s\n", test->d_name);
		if (ft_strncmp(test->d_name, ".", 1) != 0 && ft_strncmp(test->d_name, "..", 2) != 0  && flag == 0)
		{
			temp[k] = ft_strdup(test->d_name);
			//printf("k = %s\n", temp[k]);
			k++;
		}
		else if (ft_strncmp(test->d_name, ".", 1) != 0 && ft_strncmp(test->d_name, "..", 2) != 0 && flag == 1)
		{
			if (ft_strncmp(rev_string(test->d_name), rev_string(argument), ft_strlen(argument) - i - 1) == 0)
			{
				temp[k] = ft_strdup(test->d_name);
				//printf("sto cazzo = %s\n", temp[k]);
				k++;
			}
			
		}
		else if (ft_strncmp(test->d_name, ".", 1) != 0 && ft_strncmp(test->d_name, "..", 2) != 0 && flag == 2)
		{
			if (ft_strchr(test->d_name, '.') != 0)
			{
				temp[k] = ft_strdup(test->d_name);
				//printf("sto cazzone = %s\n", temp[k]);
				k++;
			}
		}
		else if (ft_strncmp(test->d_name, ".", 1) != 0 && ft_strncmp(test->d_name, "..", 2) != 0 && flag == 3)
		{
			if (counting_cards(test->d_name) >= c)
			{
				temp[k] = ft_strdup(test->d_name);
				//printf("sto cazzetto = %s\n", temp[k]);
				k++;
			}
		}
		else if (ft_strncmp(test->d_name, ".", 1) != 0 && ft_strncmp(test->d_name, "..", 2) != 0 && flag == 4)
		{
			if (counting_cards(test->d_name) >= c && (ft_strncmp(rev_string(test->d_name), rev_string(argument), ft_strchr(rev_string(argument), '.') + 1) == 0))
			{
				temp[k] = ft_strdup(test->d_name);
				//printf("sto cazzino = %s\n", temp[k]);
				k++;
			}
		}
		test = readdir(dir);
	}
	// temp[k] = NULL;
	//prova = remove_shit_from_directory_files(temp);
	// int j = -1;
	// while (temp[++j])
		//printf("temp = %s\n", temp[j]);
	closedir(dir);
	return (temp);
}

int reading_directory(char  *cartella)
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
		//printf("open_dir\n");
		if (ft_strncmp(test->d_name, ".", 1) != 0 && ft_strncmp(test->d_name, "..", 2) != 0)
			nb++;
		test = readdir(dir);
	}
	//printf("nb = %d\n", nb);
	closedir(dir);
	return (nb);
}
