# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joockim <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/10 16:22:30 by joockim           #+#    #+#              #
#    Updated: 2021/01/09 01:31:17 by joockim          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc

UTILS = ./libs/

EXCUTE_PATH = ./srcs/

#CFLAGS = -Werror -Wextra -Wall

LIBFT_FLAGS = -L ${UTILS}libft -lft -L ${UTILS}libftprintf -lftprintf

FLAGS = $(LIBFT_FLAGS)

SRCS = $(addprefix ./srcs/, \
	command_work.c \
	unset_work.c \
	split_separator.c \
	list_check.c \
	error_check.c \
	utils.c \
	work_redir.c \
	commands_addback.c \
	get_word.c \
	get_word_utils.c \
	word_init.c \
	get_basic.c \
	get_quotation.c \
	change_env.c \
	change_env_utils.c \
	parse_node.c \
	make_path_lst.c \
	free_nodes.c \
	input_sequence.c \
	env_func_1.c \
	env_func_2.c \
	prompt_utils.c \
	utils2.c \
	minishell.c)

OBJS = $(SRCS:.c=.o)

EXCUTABLE = pwd echo env

.phony : all clean fclean re

all : ${NAME}

${NAME} : ${OBJS}
	@make bonus -C ${UTILS}libft
	@make -C ${UTILS}libftprintf
	@${CC} ${FLAGS} ${OBJS} -o ${NAME}
	@${CC} ${FLAGS} -o pwd ${EXCUTE_PATH}excute_pwd.c
	@${CC} ${FLAGS} -o echo ${EXCUTE_PATH}excute_echo.c
	@${CC} ${FLAGS} -o env ${EXCUTE_PATH}excute_env.c
	@echo ----------make success minishell----------

clean :
	@make clean -C ${UTILS}libft
	@make clean -C ${UTILS}libftprintf
	@rm -f ${OBJS}
	@rm -f ${EXCUTABLE}
	@echo ------------do clean------------

fclean : clean	
	@make fclean -C ${UTILS}libft
	@make fclean -C ${UTILS}libftprintf
	@rm -f ${NAME}
	@echo ------------do fclean------------

re : fclean all
