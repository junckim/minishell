# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joockim <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/10 16:22:30 by joockim           #+#    #+#              #
#    Updated: 2020/12/11 14:33:46 by kimjoocha        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc

UTILS = ./utils/

LIBFT_FLAGS = -L ${UTILS}libft -lft -L ${UTILS}libftprintf -lftprintf

SRCS = minishell.c

OBJS = $(SRCS:.c=.o)

.phony : all clean fclean re

all : ${NAME}

${NAME} : ${OBJS}
	@make -C ${UTILS}libft
	@make -C ${UTILS}libftprintf
	@${CC} ${LIBFT_FLAGS} ${OBJS} -o ${NAME}
	@echo ----------make success minishell----------

.c.o :
	@$(CC) -c $<

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
