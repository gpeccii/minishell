/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcologgi <dcologgi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 12:11:37 by dcologgi          #+#    #+#             */
/*   Updated: 2023/06/26 15:53:26 by dcologgi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_envp(t_shell *mini, char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
		i++;
	mini->new_envp = malloc((i + 2) * sizeof(char *));
	i = 0;
	while (envp[i])
	{
		mini->new_envp[i] = ft_strdup(envp[i]);
		i++;
	}
}

void	create_new_var(t_shell *mini, char *str)
{
	char	*name;
	char	*value;
	char	*new_var;
	int		i;

	i = 0;
	name = get_env_name(str);
	value = get_env_value(str);
	new_var = malloc((ft_strlen(name) + ft_strlen(value) + 2) * sizeof(char));
	ft_strcpy(new_var, name);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, value);
	while (mini->new_envp[i])
		i++;
	mini->new_envp[i] = new_var;
	mini->new_envp[i + 1] = NULL;
	free(name);
}

char	*get_env_name(char *env)
{
	int		i;
	char	*name;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	name = malloc((i + 1) * sizeof(char));
	ft_strcpy(name, env);
	name[i] = '\0';
	return (name);
}

char	*get_env_value(char *env)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	return (ft_strdup(env + i + 1));
}

void	swap_envp(t_shell *mini, int i, int k)
{
	char	*temp;

	temp = mini->new_envp[i];
	mini->new_envp[i] = mini->new_envp[k];
	mini->new_envp[k] = temp;
}
