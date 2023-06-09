# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/09 15:52:20 by gpecci            #+#    #+#              #
#    Updated: 2023/06/09 16:15:15 by gpecci           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

MAKEFLAGS += --silent

LIBFT_PATH = libft/
LIBFT = $(LIBFT_PATH)libft.a

P1 = parser/

SRCS =	$(P1)new_split.c \
		main.c \

OBJS = ${SRCS:.c=.o}

CC		= gcc
RM		= rm -f

CFLAGS = -Wall -Wextra -Werror

.c.o:
		${CC} ${CFLAGS} -g -c $< -o ${<:.c=.o}

$(NAME):	$(OBJS) $(LIBFT)
			$(CC) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
	@echo "\033[32mMinishell Ready!\033[0m"

$(LIBFT):
			make -C $(LIBFT_PATH)


all : $(NAME)

clean:
		${RM} ${OBJS}
		make clean -C ${LIBFT_PATH}
	@echo "\033[33mclean OK!\033[0m"

fclean:		clean
				rm -f $(NAME)
				rm -f ${LIBFT}
				@echo "\033[33mfclean OK!\033[0m"

re:				fclean all
