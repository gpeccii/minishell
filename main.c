/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:27:21 by gpecci            #+#    #+#             */
/*   Updated: 2023/06/09 17:08:52 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	**matrix;
	t_mini	mini;
	int		i;

	while (1)
	{
		mini.input = readline("Minishell>");
		add_history(mini.input);
		matrix = ft_newsplit(mini.input, 32);
		i = 0;
		while (matrix[i])
		{
			printf("%s\n", matrix[i]);
			i++;
		}
	}
	return (0);
}
