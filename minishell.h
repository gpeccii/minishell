/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbordin <rbordin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 12:16:32 by rbordin           #+#    #+#             */
/*   Updated: 2023/06/30 15:29:30 by rbordin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <string.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <dirent.h>
# include <inttypes.h>
# include <sys/wait.h>
# include "get_next_line/get_next_line.h"


typedef struct s_pipe
{
	int num_pipes;
	int i;
	int *fds;
} t_pipex ;
typedef struct s_flags
{
	int	command_separator;
	int	operators;
	int	major;
	int	minor;
	int	quote_flag;
	int	redirect_input;
	int	redirect_output;
	int	brackets;
	int	pipes;
}	t_flags;

typedef struct s_args
{
	char			*str;
	char			*command;
	char			*flags;
	char			*argument;
	char			*redirect;
	char			*infile;
	char			*outfile;
	int				order;
	int				redirection_quantity;
	struct s_args	*previous;
	struct s_args	*next;
}	t_args;

typedef struct s_shell
{
	char		*home;
	char		*input;
	char		**buffer;
	char		**commands;
	int			command_presence;
	char		**operators;
	char		*main_path;
	char		**envp;
	char		**new_envp;
	char		*syntax;
	char		*paths;
	char		**dollar;
	int			dollar_flag;
	int			exit;
	int			total_pipes;
	int			flag_status;
	t_flags		flags;
	t_args		*list;
}	t_shell;

// general_utils.c
int				ft_revstr(char *str, char c);
char			*ft_strcat(char *dest, const char *src);
char			*ft_strcpy(char *dest, const char *src);
char			*rev_string(char *s);

//stronzo.c
void	init_flags(t_shell *mini);
void	print_error(char *error);

//split_strings.c
void	splitting_operators(t_shell *mini);
t_args	*ft_lstnew_mini(void);
void	ft_lstadd_back_mini(t_args **lst, char *command);

//parser.c
void	analizer(t_shell *mini, t_args *node, char **envp);
char	*check_path(t_shell *mini, char *command);
int		check_redirection(char *s);
void	free_matrix(char **matrix);
void	redirecting_nodes(t_args *cur, t_args *new, t_args *red);
int	check_path2(t_shell *mini, char *s);

//parser_utils.c
void	checking_node_vilidity(t_shell *mini, t_args *node);
void	ultimating_commands(t_shell *mini, t_args **head);
int		reassembling_strings(char **temp, t_args *node, int i);
void	its_a_command(t_shell *mini, t_args *node, char *temp);
void	its_not_a_command(t_shell *mini, t_args *node, char *temp, int i);
void	boh(t_shell *mini, char *temp, t_args *node);
int 	reassembling_brackets(char **temp, t_args *node, int i);
void	its_a_flag(t_shell *mini, t_args *node, char *temp);

//lists.c
t_args*	create_node(char *data);
void	insert_last_with_delimiter(t_shell *mini, t_args** head, char **envp, char delim);
void	print_list(t_args* head);
void	init_node(t_args *node);
void	handling_dollar(t_shell *mini, t_args *head);

//ft_mini_split.c
char	**ft_mini_split(t_shell *mini, char const *s, char c);
char	**creting_matrix_to_populate_nodes(char **tokens);
int		operator_checker(const char *s, char control);
char	*ft_strjoin_mini(char const *s1, char const *s2, int free1, int free2);
int		checking_quotes_for_operator(const char *s, char control, int pos);
char	**new_function(char **matrix);

//test_input.c
int	start(t_shell *mini);
char	*insertSpaces(char *str, int len);
int		isDelimiter(char c, char *str, int i);
char	*getting_final_string(char *s, char c);
int		counting(char *s);

//expander.c
char	*expanding_dollar_own_variables(t_shell *mini, char *s);
void	replacer(t_shell *mini, t_args *node);
void	saver(t_shell *mini, t_args *node);
void	get_dollars(t_shell *mini, char *s);
char	*expanding_d(t_shell *mini, char *s);

//lists_utils.c
void	test(t_args **head, t_args *new_node);
void clear_list(t_args **head);
void	adding_rediretion_node(t_shell *mini, t_args **node);

//wildcats.c
void    wild(t_shell *mini, t_args **node);
char	**cards(char *argument, int i, int c);
int reading_directory(char *cartella);
char	**remove_shit_from_directory_files(char **temp);
int	counting_cards(char *argument);

// signal.c
void			handlectrl(t_shell *mini, char **envp);
void			handlectrlc(int signal);

// builtin_ctrl.c
int				check_builtin_presence(t_shell *mini, char *command);
void			builtin_exec(t_shell *mini, char **envp, t_args *node,
					char *temp);

// builtin_exec.c
void			command_echo(t_shell *mini, char **envp, t_args *current);
void			command_cd(t_shell *mini, t_args *node, char *arg);
void			command_pwd(t_shell *mini);
void			command_env(t_shell *mini, char **envp);

// export_unset.c
void			command_export(t_shell *mini, char **envp, char *str);
void			command_unset(t_shell *mini, char *str);

// export_utils.c
char			*get_env_value(char *env);
char			*get_env_name(char *env);
void			swap_envp(t_shell *mini, int i, int k);
void			create_new_var(t_shell *mini, char *str);
void			copy_envp(t_shell *mini, char **envp);

//pipe.c
int    pipe_set_up(t_shell *mini, t_args **node);
void    pipe_init(t_shell *mini, t_args **node);
static char **create_matri(t_args *node);
void	execpipe(t_shell *mini, t_args  *node);
void	single(t_shell *mini, t_args *node, char **envp);
void    screening_terminal(t_shell *mini, t_args *node, int temp_fd);
//builtins_pipe
void builtin_pipe(t_shell *mini, t_pipex *pipes, t_args *cur);
void init_pipexxx(t_pipex *pipes, t_shell *mini, t_args *cur);



char	**ft_echo_split(t_shell *mini, char const *s, char c);

void	echo_replacer(t_shell *mini, t_args *node);


#endif
