/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbordin <rbordin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:11:11 by dcologgi          #+#    #+#             */
/*   Updated: 2023/06/30 12:04:58 by rbordin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	command_echo(t_shell *mini, char **envp, t_args *current)
{
	if ((current->argument == NULL && current->flags) || (current->argument
			&& (ft_strncmp(current->argument, "\"-n\"", 4) == 0)))
		return ;
	else if (current->argument == NULL && !current->flags)
	{
		write(1, "\n", 1);
		return ;
	}
	if (current->argument[0] == '\"' && current->argument != NULL)
		current->argument = ft_strtrim(current->argument, "\"");
	if (current->argument[0] == '\'' && current->argument != NULL)
	{
		current->argument = ft_strtrim(current->argument, "\'");
		write(1, current->argument, ft_strlen(current->argument));
		if (!current->flags)
			write(1, "\n", 1);
		return ;
	}
	echo_replacer(mini, current);
	write(1, current->argument, ft_strlen(current->argument));
	if (!current->flags)
		write(1, "\n", 1);
}

void	command_env(t_shell *mini, char **envp)
{
	int		j;
	char	*dollar;
	char	*argument;
	t_shell	*temp;

	j = 0;
	dollar = "$";
	temp = mini;
	while (envp[j] != NULL)
	{
		argument = ft_strjoin(dollar, envp[j], NO_FREE, NO_FREE);
		if (!temp->list->argument)
			printf("%s\n", envp[j]);
		else if (ft_strncmp(argument, temp->list->argument,
				ft_strlen(temp->list->argument)) == 0)
			printf("%s\n", envp[j]);
		j++;
	}
}

void	command_pwd(t_shell *mini)
{
	char	pwd_path[1024];

	if (getcwd(pwd_path, sizeof(pwd_path)) != NULL)
		printf("%s\n", pwd_path);
}

static void	create_path(t_shell *mini, char *path)
{
	mini->main_path = ft_strtrim(mini->main_path, "$ ");
	mini->main_path = ft_strjoin(mini->main_path, "/", FREE,
			NO_FREE);
	mini->main_path = ft_strjoin(mini->main_path, path, FREE,
			NO_FREE);
	mini->main_path = ft_strtrim(mini->main_path, "/");
	mini->main_path = ft_strjoin(mini->main_path, "$ ", FREE,
			NO_FREE);
	//chdir(path);
}

void	command_cd(t_shell *mini, t_args *node, char *arg)
{
	char	*curr_path;
	char	*path;

	curr_path = getenv("PWD");
	if (!arg)
		path = getenv("HOME");
	else
		path = arg;
	if (chdir(path) == -1)
		printf("chdir error: Bad address %s\n", path);
	else
	{
		if (ft_strncmp(path, "..", 2) == 0)
		{
			//opendir("..");

			mini->main_path =ft_substr(mini->main_path, 0, ft_strlen(mini->main_path) - ft_strchr(rev_string(mini->main_path), '/') - 1);
			mini->main_path = ft_strjoin(mini->main_path, mini->syntax, FREE,
					NO_FREE);
		}
		else
			create_path(mini, path);
	}
	//closedir((DIR*)path);
}