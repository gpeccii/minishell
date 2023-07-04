/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbordin <rbordin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 16:20:15 by riccardobor       #+#    #+#             */
/*   Updated: 2023/06/30 14:15:57 by rbordin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int 	g_exit_status;

void	checking_node_vilidity(t_shell *mini, t_args *node)
{
	t_args *cur;
	t_args *new;
	t_args *red;
	//printf("checking_node_validity\n");
	red = malloc(sizeof(t_args));
	new = malloc(sizeof(t_args));
	cur = node;

	if (cur->redirect != NULL)
	{
		new->next = cur->next;
		cur->next = red;
		red->next = new;
		red->previous = cur;
		new->previous = red;
		redirecting_nodes(cur, new, red);
		mini->command_presence = 0;
		analizer(mini, new, NULL);
		// if (cur->outfile != NULL)
		// {
		// 	if (strcmp(cur->outfile, new->outfile) == 0)
		// 		cur->outfile = NULL;
		// }
		return ;
	}
	else
	{
		free(red);
		free(new);
	}
}


int reassembling_strings(char **temp, t_args *node, int i)
{
	int	j;

	j = i;
	// if (ft_strlen(temp[j]) == 1)
	// 	j++;
	while (temp[j] && (temp[j][ft_strlen(temp[j]) - 1] != temp[i][0]))
	{
		//printf("strings\n");
		if (node->argument == NULL)
			node->argument = ft_strdup(temp[j]);
		else
			node->argument = ft_strjoin_mini(node->argument, temp[j], FREE, NO_FREE);
		
		j++;
	}
	if (temp[j] && (temp[j][ft_strlen(temp[j]) - 1] == temp[i][0]))
	{
		if (node->argument == NULL)
			node->argument = ft_strdup(temp[j]);
		else
			node->argument = ft_strjoin_mini(node->argument, temp[j], FREE, NO_FREE);
	}
	return (j);
}



void	ultimating_commands(t_shell *mini, t_args **head)
{
	t_args	*cur;
	int		k;

	k = 0;
	//printf("ultimating_commands\n");
	cur = *head;
	////printf("node command = %s\n", cur->command);
	while (cur)
	{
		cur->order = k;
		if (cur->redirect && (strcmp(cur->redirect, "||") == 0 || strcmp(cur->redirect, "&&") == 0))
		{
			k++;
			cur->order = k;
			k++;
		}
		if (cur->command && cur->command[0] != '/' && check_builtin_presence(mini, cur->command) != 0)
		{
			cur->command = check_path(mini, cur->command);
		}
		cur = cur->next;
	}
	// if (cur->command)
	// 	cur->command = check_path(mini, cur->command);
	// cur->order = k;
}




void	its_a_command(t_shell *mini, t_args *node, char *temp)
{
	//printf("it's a command= %s\n", node->command);
	if (mini->command_presence == 0 &&  node->command == NULL  && mini->flags.minor == 0 && mini->flags.redirect_input == 0)
	{
		mini->command_presence = 1;
		////printf("command1 = %s\n", node->command);
		node->command = ft_strdup(temp);
		//("command2 = %s\n", node->command);
		//printf("------------------------%d\n", mini->command_presence);
	}
	else
	{
		if (node->argument != NULL)
			node->argument = ft_strjoin_mini(node->argument, temp, NO_FREE, NO_FREE); 
		else
			node->argument = ft_strdup(temp);
		//////printf("not a command\n");
	}
}

void	its_not_a_command(t_shell *mini, t_args *node, char *temp, int i)
{
	int res;
	
	res = 0;
	res = check_redirection(temp);
	if (res == -2)
	{
		mini->exit = 1;
		return ;
	}
	//printf("res = %d\n", res);
	//printf("command pres = %d\n", mini->command_presence);
	if (res > 0) 
	{
		if (res == 1 && node->infile == NULL)
			mini->flags.redirect_input = 1;
		else if (res == 2 && node->infile == NULL)
			mini->flags.minor = 1;
		else if (res == 3 && node->outfile == NULL)
			mini->flags.redirect_output = 1;
		else if (res == 4 && node->outfile == NULL)
			mini->flags.major = 1;
		else if (res == 6)
			mini->flags.pipes = 1;
		if (ft_strlen(temp) < 3 && temp[0] != '>' && temp[0] != '<')
		{
			node->redirect = ft_strdup(temp);
			mini->command_presence = 0;
		}
		else if (ft_strlen(temp) >= 3 && node->argument == NULL)
			node->argument = ft_strdup(temp);
		else if (temp[0] != '>' && temp[0] != '<')
		{
			node->argument = ft_strjoin_mini(node->argument, temp, NO_FREE, NO_FREE);
		}
		if (res == 2 || res == 4)
			node->redirection_quantity = 1;
		else if (res == 1 || res == 3)
			node->redirection_quantity = 2;
		return ;
	}
	if (node->command == NULL && mini->command_presence == 0 && mini->flags.minor == 0)
	{
		//("$$$$$$$%s\n", temp);
		if (mini->flags.major == 0 && mini->flags.redirect_output == 0)
		{
			print_error("command not found");
			mini->exit = 1;
			mini->flag_status = 127;
			return ;
		}
	}
	else if(node->command != NULL && mini->flags.redirect_input == 1 && temp[0] != '<')
	{
		//write(1, "PIPPOOOOOO\n", 11);
		node->infile = ft_strdup(temp);
		mini->flags.redirect_input = 0;
	}
	else if (mini->command_presence != 0 && mini->flags.minor == 1)
	{
		node->infile = ft_strdup(temp);
		mini->flags.minor = 0;
	}
	else if (mini->command_presence == 0 && mini->flags.minor == 1)
	{
		if (temp[0] == '<')
			return;
		else
		{
			node->infile = ft_strdup(temp);
			mini->flags.minor = 0;
			mini->flags.redirect_input = 0;
		}
	}
	if (node->command != NULL && (mini->flags.major == 1 || mini->flags.redirect_output == 1) && temp[0] != '>')
	{
		node->outfile = ft_strdup(temp);
		mini->flags.redirect_output = 0;
		mini->flags.major = 0;
	}
	if (node->argument != NULL && (node->outfile == NULL) && temp[0] != '>' && temp[0] != '<')
		node->argument = ft_strjoin_mini(node->argument, temp, FREE, NO_FREE);
	//printf("argument = %s\n", temp);
	if (node->argument == NULL && (node->outfile == NULL && node->infile == NULL) && temp[0] != '>' && temp[0] != '<')
	{
		//printf("AAAAAAAAAAAAAAAAAAA\n");
		node->argument = ft_strdup(temp);
	}
}


void	boh(t_shell *mini, char *temp, t_args *node)
{
	int j;

	j = 0;
	while (node->str[j] == ' ')
		j++;
	while (node->str[j] && node->str[j] != ' ')
		j++;
	while (node->str[j] == ' ')
		j++;
	if (node->str[j] != '<')
	{
		("Minishell:\n");
		print_error("command not found");
		mini->exit = 1;
		mini->flag_status = 127;
		return ;
	}
	else
		node->infile = ft_strdup(temp);
}

int reassembling_brackets(char **temp, t_args *node, int i)
{
	int	j;

	j = i;
	while (temp[j] && (temp[j][ft_strlen(temp[j]) - 1] != ')'))
	{
		if (node->argument == NULL)
			node->argument = ft_strdup(temp[j]);
		else
			node->argument = ft_strjoin_mini(node->argument, temp[j], FREE, NO_FREE);
		
		j++;
	}
	if (temp[j] && (temp[j][ft_strlen(temp[j]) - 1] == ')'))
	{
		if (node->argument == NULL)
			node->argument = ft_strdup(temp[j]);
		else
			node->argument = ft_strjoin_mini(node->argument, temp[j], FREE, NO_FREE);
	}
	return (j);
}

void	its_a_flag(t_shell *mini, t_args *node, char *temp)
{
	//printf("it's a command= %s\n", node->command);
	if (node->flags == NULL )
		node->flags = ft_strdup(temp);
	else
	{
		if (node->argument != NULL)
			node->argument = ft_strjoin_mini(node->argument, temp, NO_FREE, NO_FREE); 
		else
			node->argument = ft_strdup(temp);
			
	}
}