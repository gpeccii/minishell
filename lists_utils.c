/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 11:58:33 by riccardobor       #+#    #+#             */
/*   Updated: 2023/07/06 17:13:50 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test(t_args **head, t_args *new_node)
{
	t_args	*last;

	last = *head;
	while (last->next != NULL)
		last = last->next;
	last->next = new_node;
	new_node->previous = last;
	new_node->order = last->order + 1;
	new_node->next = NULL;
}

void	clear_list(t_args **head)
{
	t_args	*current;
	t_args	*next;

	current = *head;
	while (current != NULL)
	{
		next = current->next;
		if (current->str != NULL)
			free(current->str);
		if (current->command != NULL)
			free(current->command);
		if (current->flags != NULL)
			free(current->flags);
		if (current->argument != NULL)
			free(current->argument);
		if (current->redirect != NULL)
			free(current->redirect);
		free(current);
		current = next;
	}
	*head = NULL;
}

void	init_node(t_args *node)
{
	node->command = NULL;
	node->flags = NULL;
	node->argument = NULL;
	node->redirect = NULL;
	node->str = NULL;
	node->infile = NULL;
	node->outfile = NULL;
	node->redirection_quantity = 0;
}
