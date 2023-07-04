# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rbordin <rbordin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/30 15:11:26 by rbordin           #+#    #+#              #
#    Updated: 2023/06/30 16:38:20 by rbordin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIBFT_PATH = libft

GNL = get_next_line

LIBFT = $(LIBFT_PATH)/libft.a

MAKEFLAGS += --silent

SRC = dick.c\
	ft_mini_split.c \
	lists.c \
	parser.c \
	parser_utils.c \
	test_input.c \
	expander.c \
	lists_utils.c \
	wildcats.c \
	builtin_ctrl.c \
	builtin_exec.c \
	export_unset.c \
	export_utils.c \
	general_utils.c \
	signal.c \
	pipe.c \
	$(GNL)/get_next_line.c \
	$(GNL)/get_next_line_utils.c \
	builltin_pipe.c \
	ft_echo_split.c \
	echo_replacer.c \

OBJ = ${SRC:.c=.o}

CC = gcc

RM = rm -f

CFLAGS = -W -W -W


.o:.c
		${CC} ${CFLAGS} -g -c $< -o ${<:.c=.o}

${NAME}:  ${OBJ}
	make -C $(LIBFT_PATH)
	${CC} $(CFLAGS) ${OBJ} ${LIBFT} -lreadline -o $(NAME)
	@echo "\033[32mCompiled OK!\033[0m"
	
all: ${NAME}

clean:
		${RM} ${OBJ}
		make clean -C ${LIBFT_PATH}
		@echo "\033[35mCleared everything!\033[0m"
		
fclean: clean
		make fclean -C ${LIBFT_PATH}
		${RM} ${NAME}

re: fclean all