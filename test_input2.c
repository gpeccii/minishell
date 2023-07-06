/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_input2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 17:19:38 by gpecci            #+#    #+#             */
/*   Updated: 2023/07/06 17:19:42 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_delimiter(char c, char *str, int i)
{
	return (c == '|' || c == '&');
}

char	*getting_final_string(char *s, char c)
{
	t_varie	var;

	init_varie(&var, 0);
	var.i = 0;
	var.newStr = ft_calloc(ft_strlen(s) + counting(s) + 1, sizeof(char));
	while (s[var.i])
	{
		if (s[var.i] == c && c != '<' && c != '>')
			first_half(&var, s, c);
		else if (s[var.i] == '<' || s[var.i] == '>')
			sencond_half(&var, s, c);
		var.newStr[var.z] = s[var.i];
		var.i++;
		var.z++;
	}
	var.newStr[var.z] = '\0';
	free(s);
	return (var.newStr);
}

int	counting(char *s)
{
	int	i;
	int	k;

	k = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '|' || s[i] == '&' || s[i] == '<' || s[i] == '>')
		{
			if (checking_quotes_for_operator(s, s[i], i))
				k++;
		}
		i++;
	}
	return (k);
}

int	start(t_shell *mini)
{
	int	len;

	len = strlen(mini->input);
	if (len == 0 || !mini->input)
		return (0);
	if (len > 0 && mini->input[len - 1] == '\n')
	{
		mini->input[len - 1] = '\0';
		len--;
	}
	mini->input = insert_spaces(mini->input, len);
	mini->input = getting_final_string(mini->input, '|');
	mini->input = getting_final_string(mini->input, '&');
	mini->input = getting_final_string(mini->input, '<');
	mini->input = getting_final_string(mini->input, '>');
	return (1);
}

void	init_varie(t_varie *varie, int len)
{
	varie->i = -1;
	varie->j = 0;
	varie->z = 0;
	varie->newLen = len;
	varie->withinQuotes = 0;
	varie->withinDoubleQuotes = 0;
	varie->quotesStack = 0;
	varie->doubleQuotesStack = 0;
	varie->newStr = NULL;
}
