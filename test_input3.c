/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_input3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 17:19:54 by gpecci            #+#    #+#             */
/*   Updated: 2023/07/06 17:20:02 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_half(t_varie *var, char *s, char c)
{
	var->newStr[var->z++] = s[var->i];
	var->j = var->i + 1;
	while (s[var->j] == ' ')
	{
		if (s[var->j] == '\0')
			break ;
		var->j++;
	}
	if (s[var->j] != c)
	{
		var->newStr[var->z++] = ' ';
		var->i = var->j;
	}
	else
	{
		var->i = var->j;
		var->newStr[var->z++] = s[var->i++];
		var->newStr[var->z++] = ' ';
	}
}

void	sencond_half(t_varie *var, char *s, char c)
{
	var->newStr[var->z++] = ' ';
	if (s[var->i + 1] != ' ' )
	{
		var->newStr[var->z++] = s[var->i++];
		if (s[var->i] == '<' || s[var->i] == '>')
			var->newStr[var->z++] = s[var->i++];
		if (s[var->i] != '<' && s[var->i] != '>')
			var->newStr[var->z++] = ' ';
	}
}
