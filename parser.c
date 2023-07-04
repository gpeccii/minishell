/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbordin <rbordin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 09:45:37 by rbordin           #+#    #+#             */
/*   Updated: 2023/06/30 12:45:31 by rbordin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    analizer(t_shell *mini, t_args *node, char **envp)
{
	int i;
	int j;
	int res;
	char **temp;
	
	i = 0;
	j = 0;
	//printf("analizer\n");
	temp = ft_echo_split(mini, node->str, ' ');
	
		 
	
	if (!checking_quotes_for_operator(node->str, '\'', 1) || !checking_quotes_for_operator(node->str, '\"', 1))
		node->argument = ft_strdup(node->str);
	else
	{
		while (temp[i])
		{
			//write(1, temp[i], ft_strlen(temp[i]));
			//write(1, "\n", 1);
			//printf("temp[%d] = %s\n", i, temp[i]);
			res = 0;
			if (!checking_quotes_for_operator(temp[i], '\'', 1) || !checking_quotes_for_operator(temp[i], '\"', 1))
				node->argument = ft_strdup(temp[i]);
			else if (temp[i][j] == '(')
				i = reassembling_brackets(temp, node, i);
			else if (temp[i][j] == '-')
			{
				if (node->argument == NULL)
					its_a_flag(mini, node, temp[i]);
			}
			else if (check_builtin_presence(mini,temp[i]) == 0 && node->command == NULL)
			{
				//("builtin222222222\n");
				node->command = ft_strdup(temp[i]);
				mini->command_presence = 1;
			}
			else if(temp[i][j] == '\'' || temp[i][j] == '\"')
				i = reassembling_strings(temp, node, i);
			else if (temp[i][j] == '/' && check_path2(mini, temp[i]) == 1 && check_builtin_presence(mini,temp[i]) == 1)
				its_a_command(mini, node, temp[i]);
			else if (check_path(mini, temp[i]) != NULL && check_builtin_presence(mini,temp[i]) == 1)
				its_a_command(mini, node, temp[i]);
			else if (check_path(mini, temp[i]) == NULL)
			{
				its_not_a_command(mini, node, temp[i], i);
				if (mini->exit == 1)
				{
					free_matrix(temp);
					return ;
				}
			}
			if (strcmp(node->str, "||") == 0 || strcmp(node->str, "&&") == 0)
			{
				mini->flags.command_separator = 1;
				mini->command_presence = 0;
			}
			if (temp[0][0] == '$' && node->order == 0)
				mini->dollar_flag = 1;
			i++;
			if (temp[i] == NULL && node->command != NULL)
				checking_node_vilidity(mini, node);
		}
		if (node->command == NULL && node->redirect == NULL)
		{
			//printf("$$$$$$$2%s\n", node->command);

			mini->exit = 1;
			print_error("command not found");
		}
	}
	free_matrix(temp);
}

int	check_path2(t_shell *mini, char *s)
{
	if (access(s, F_OK) == 0)
		return (1);
	return (0);
}

int	check_redirection(char *s)
{
	int i;
	int	k;

	k = 0;
	i = 0;
	//printf("check_redirection\n");
	while (s[i])
	{
		if (s[i] == '<')
			if (checking_quotes_for_operator(s, '\'', i) || checking_quotes_for_operator(s, '\"', i))
				k = 2;
		if (s[i] == '>')
			if (checking_quotes_for_operator(s, '\'', i) == 1 || checking_quotes_for_operator(s, '\"', i) == 1)
				k = 4;
		if (s[i] == '|')
			if (checking_quotes_for_operator(s, '\'', i) == 1 || checking_quotes_for_operator(s, '\"', i) == 1)
				k = 6;
		if (s[i] == '&')
			if (checking_quotes_for_operator(s, '\'', i) == 1 || checking_quotes_for_operator(s, '\"', i) == 1)
				k = 8;
		if (k != 0)
			break;
		i++;
	}
	if (k!= 0 && s[i + 1] == s[i] && s[i + 2] != s[i])
		k -= 1;
	else if (k!= 0 && s[i + 1] == s[i] && s[i + 2] == s[i])
	{
		printf("Minishell: syntax error near unexpected token '%c'\n", s[i]);
		return (-2);
	}
	return (k);
}

char *check_path(t_shell *mini, char *command)
{
	char    *path;
	char    *support;
	char    **temp;
	int     i;
	int     flag;
	

	flag = 0;
	i = -1;
	path = getenv("PATH");
	temp = ft_split(path, ':');
	while (temp[++i])
	{
		temp[i] = ft_strjoin(temp[i], "/", NO_FREE, NO_FREE);
		support = ft_strjoin(temp[i], command, NO_FREE, NO_FREE);
		if (access(support, F_OK) == 0)
		{
			flag = 1;
			break;
		}
		free(support);
	}
	free_matrix(temp);
	if (flag == 1)
		return (support);
	return (NULL);
}

void    free_matrix(char **matrix)
{
	int i;

	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

static void	checking_args(t_args *cur)
{
	int	i;
	//printf("\nchecking args\n");
	i = 0;
	while (cur->argument)
	{
		//printf("\nin while\n");
		if (cur->argument[i] == '\0')
		{
			free(cur->argument);
			cur->argument = NULL;
		}
		else if (cur->argument[i] != ' ')
			break ;
		i++;
	}
}

void	redirecting_nodes(t_args *cur, t_args *new, t_args *red)
{
	int	k;
	
	k = 0;
	if (cur->flags != NULL)
		k = ft_strlen(cur->flags);
	init_node(red);
	init_node(new);
	cur->str = ft_strtrim(cur->str, " ");
	red->str = ft_strdup(cur->redirect);
	cur->redirect = NULL;
	red->redirect = ft_strdup(red->str);
	new->str = ft_substr(cur->str, ft_strchr(cur->str, red->str[0]) + 1, ft_strlen(cur->str) - ft_strchr(cur->str, red->str[0]));
	//printf("---------%s\n", cur->str);
	if (ft_strchr(cur->str, '|') < ft_strchr(cur->str, '>'))
	{
		cur->outfile = NULL;
		cur->redirection_quantity = 0;
	}
	if (ft_strchr(cur->str, '|') < ft_strchr(cur->str, '<'))
	{
		cur->infile = NULL;
		cur->redirection_quantity = 0;
	}
	if (cur->infile != NULL && cur->outfile != NULL)
	{
		if (ft_strchr(cur->str, '<') < ft_strchr(cur->str, '>'))
			cur->outfile = NULL;
		else
			cur->infile = NULL;
	}
	// if (cur->infile == NULL && cur->outfile == NULL)
	if (cur->infile != NULL && ft_strchr(cur->str, '<') != -1)
	{
		cur->argument = ft_strdup(cur->infile);
		if (cur->str[ft_strchr(cur->str, '<') + 1] == '<')
			cur->argument = NULL;
	}
	else if (cur->outfile != NULL && ft_strchr(cur->str, '>') != -1)
	{
		cur->argument = ft_substr(cur->str, ft_strlen(cur->command) + 1, ft_strchr(cur->str, '>') - ft_strlen(cur->command) - 1);
		cur->argument = ft_strtrim(cur->argument, " ");
		// if (cur->str[ft_strchr(cur->str, '>') + 1] == '>')
		// 	cur->argument = ft_substr(cur->str, ft_strlen(cur->command) + 1, ft_strchr(cur->str, '>') - ft_strlen(cur->command));

	}
	else
	{
		cur->argument = ft_substr(cur->str, ft_strlen(cur->command) + k + 1, ft_strchr(cur->str, red->str[0]) - k - ft_strlen(cur->command) - 1);
		cur->argument = ft_strtrim(cur->argument, " ");
	}
	// else if (cur->infile != NULL)
	// 	cur->argument = ft_substr(cur->str, ft_strchr(cur->str, '<') + k + 3 + ft_strlen(cur->command), ft_strchr(cur->str, red->str[0]) - (ft_strchr(cur->str, '<') + k + 3 + ft_strlen(cur->command)));
	// else if (cur->outfile != NULL)
	// 	cur->argument = ft_substr(cur->str, ft_strlen(cur->command) + k + 1, ft_strchr(cur->str, '>') - k - ft_strlen(cur->command) - 1);
	checking_args(cur);
	//free(cur->str);
	//cur->str = ft_strdup(cur->command);
	free(cur->redirect);
}
