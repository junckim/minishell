# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joockim <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/10 16:22:30 by joockim           #+#    #+#              #
#    Updated: 2020/12/27 00:45:11 by joockim          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc

UTILS = ./libs/

#CFLAGS = -Werror -Wextra -Wall

LIBFT_FLAGS = -L ${UTILS}libft -lft -L ${UTILS}libftprintf -lftprintf

FLAGS = $(LIBFT_FLAGS)

SRCS = $(addprefix ./srcs/, \
	branch.c \
	utils.c \
	minishell.c)

OBJS = $(SRCS:.c=.o)

.phony : all clean fclean re

all : ${NAME}

${NAME} : ${OBJS}
	@make bonus -C ${UTILS}libft
	@make -C ${UTILS}libftprintf
	@${CC} ${FLAGS} ${OBJS} -o ${NAME}
	@echo ----------make success minishell----------

clean :
	@make clean -C ${UTILS}libft
	@make clean -C ${UTILS}libftprintf
	@rm -f ${OBJS}
	@echo ------------do clean------------

fclean : clean	
	@make fclean -C ${UTILS}libft
	@make fclean -C ${UTILS}libftprintf
	@rm -f ${NAME}
	@echo ------------do fclean------------

re : fclean all
