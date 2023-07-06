/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 15:39:25 by dcologgi          #+#    #+#             */
/*   Updated: 2023/07/06 17:19:01 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	first(t_varie *var, char *str, int len)
{
	while (++var->i < len)
	{
		if (str[var->i] == '\'')
		{
			var->withinQuotes = !var->withinQuotes;
			if (var->withinQuotes)
				var->quotesStack++;
			else
				var->quotesStack--;
		}
		else if (str[var->i] == '\"')
		{
			var->withinDoubleQuotes = !var->withinDoubleQuotes;
			if (var->withinDoubleQuotes)
				var->doubleQuotesStack++;
			else
				var->doubleQuotesStack--;
		}
		else if (!var->withinQuotes && !var->withinDoubleQuotes
			&& is_delimiter(str[var->i], str, var->i))
			var->newLen++;
	}
}

static void	second(t_varie *var)
{
	if (var->quotesStack != 0)
		var->newLen -= var->quotesStack;
	else
		var->newLen = var->newLen;
	if (var->doubleQuotesStack != 0)
		var->newLen -= var->doubleQuotesStack;
	else
		var->newLen = var->newLen;
	var->newStr = ft_calloc(var->newLen +2, sizeof(char));
	var->j = 0;
	var->withinQuotes = 0;
	var->withinDoubleQuotes = 0;
	var->i = -1;
}

static void	third(t_varie *var, char *str)
{
	if (var->newStr[var->j] != '|')
		var->newStr[var->j++] = ' ';
	var->newStr[var->j++] = str[var->i];
	if (str[var->i] == '|')
	{
		if (var->newStr[var->j + 1] == '|' && var->newStr[var->j - 1] != '|')
		{
			var->newStr[++var->j] = '|';
			var->j++;
		}
	}
}

static void	forth(t_varie *var, char *str)
{
	if (str[var->i] == '&')
	{
		var->newStr[var->j++] = '&';
		var->i++;
	}
	if (str[var->i] == '|')
	{
		var->newStr[var->j] = '|';
		if (var->newStr[var->j + 1] == '|')
		{
			var->newStr[++var->j] = '|';
			var->j++;
		}
	}
	var->newStr[var->j++] = str[var->i];
}

char	*insert_spaces(char *str, int len)
{
	t_varie	var;

	init_varie(&var, len);
	first(&var, str, len);
	second(&var);
	while (++var.i < len)
	{
		if (str[var.i] == '\'')
			var.withinQuotes = !var.withinQuotes;
		else if (str[var.i] == '\"')
			var.withinDoubleQuotes = !var.withinDoubleQuotes;
		if (!var.withinQuotes && !var.withinDoubleQuotes
			&& is_delimiter(str[var.i], str, var.i))
			third(&var, str);
		else
			forth(&var, str);
	}
	var.newStr[var.j] = '\0';
	return (var.newStr);
}
