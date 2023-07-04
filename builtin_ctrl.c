/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_ctrl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcologgi <dcologgi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:10:31 by dcologgi          #+#    #+#             */
/*   Updated: 2023/06/29 11:37:06 by dcologgi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_exec(t_shell *mini, char **envp, t_args *node, char *temp)
{
	if (strcmp(temp, "cd") == 0)
		command_cd(mini, node, node->argument);
	else if (strcmp(temp, "pwd") == 0)
		command_pwd(mini);
	else if (strcmp(temp, "export") == 0)
		command_export(mini, envp, node->argument);
	else if (strcmp(temp, "unset") == 0)
		command_unset(mini, node->argument);
	else if (strcmp(temp, "env") == 0)
		command_env(mini, envp);
	else if (strcmp(temp, "echo") == 0)
		command_echo(mini, envp, node);
}

int	check_builtin_presence(t_shell *mini, char *command)
{
	if (strcmp(command, "cd") == 0)
		return (0);
	else if (strcmp(command, "pwd") == 0)
		return (0);
	else if (strcmp(command, "export") == 0)
		return (0);
	else if (strcmp(command, "unset") == 0)
		return (0);
	else if (strcmp(command, "env") == 0)
		return (0);
	else if (strcmp(command, "echo") == 0)
		return (0);
	return (1);
}