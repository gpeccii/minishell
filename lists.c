/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbordin <rbordin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 14:15:03 by rbordin           #+#    #+#             */
/*   Updated: 2023/06/30 12:08:10 by rbordin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_args* create_node(char *data)
{
	t_args *new_node;
	new_node = malloc(sizeof(t_args));
	if (new_node == NULL)
		return NULL;
	init_node(new_node);
	new_node->str = ft_strdup(data);
	if (new_node->str == NULL)
	{
		free(new_node);
		return NULL;
	}
	new_node->next = NULL;
	return (new_node);
}

void insert_last_with_delimiter(t_shell *mini, t_args** head, char **envp, char delim)
{

	int i;
	char **tokens;
	t_args *new_node;
	
	tokens = ft_mini_split(mini, mini->input, delim);
	if (tokens == NULL)
		return;
	// for (int k = 0; tokens[k]; k++)
	// 	printf("]]]]]]]]]]]]]]]]]]]]]]]]]]]str = %s\n", tokens[k]);
	i = -1;
	while (tokens[++i])
	{
		new_node = create_node(tokens[i]);
		if (new_node == NULL)
			return;
		if (*head == NULL)
		{
			*head = new_node;
			new_node->order = 0;
		}
		else
			test(head, new_node);
		analizer(mini, new_node, envp);
		if (mini->exit == 1)
			return ;	
		// if (tokens[i + 1] == NULL)
		// {
		// 	t_args *end_node;
		// 	end_node = create_node("END LIST");
		// 	new_node->next = end_node;
		// }
		
	}
	//print_list(*head);
	handling_dollar(mini, *head);
	ultimating_commands(mini, head); 
}

void    handling_dollar(t_shell *mini, t_args *head)
{
	t_args *node;

	node = head;
	if (mini->dollar_flag == 1)
		saver(mini, node);
}





void print_list(t_args* head) 
{
	if (head == NULL) 
		printf("La lista è vuota.\n");
	else 
	{
		t_args* current;
		current = head;
		while (current != NULL) 
		{
			printf("node numb = %d\n", current->order);
			printf("str = %s\n", current->str);
			printf("com = %s\n", current->command);
			printf("flags = %s\n", current->flags);
			printf("arg = %s\n", current->argument);
			printf("redi = %s\n", current->redirect);
			printf("infi = %s\n", current->infile);
			printf("out = %s\n", current->outfile);
			printf("qnt = %d\n", current->redirection_quantity);

			current = current->next;
		}
		printf("NULL\n");
	}
}

