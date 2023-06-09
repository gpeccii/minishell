/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:27:14 by gpecci            #+#    #+#             */
/*   Updated: 2023/06/09 16:00:15 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include "libft/libft.h"
# include "parser/parser.h"

typedef struct s_flags
{
	int	and;
	int	parenthesis;
	int	major;
	int	minor;
	int	squote;
	int	dquote;
	int	dminor;
	int	dmajor;
	int	spipe;
	int	dpipe;
}	t_flags;

typedef struct s_args
{
	char			*str;
	char			*command;
	char			*flags;
	char			*argument;
	char			*redirect;
	struct s_args	*next;
	struct s_args	*previous;
}	t_args;

typedef struct s_mini
{
	char	*input;
	t_flags	flag;
	t_args	*list;
}	t_mini;

#endif
