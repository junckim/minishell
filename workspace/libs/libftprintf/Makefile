NAME = libftprintf.a
CC = gcc
FLAGS = -Werror -Wextra -Wall
INCLUDE = ./include/ft_printf.h
SRCS = $(addprefix ./srcs/, ft_printf.c make_flags.c utils.c case_csper.c case_p.c case_x.c case_d.c case_u.c)
OBJS = $(SRCS:.c=.o)

.PHONY : all clean fclean re

all : $(NAME)

$(NAME) : $(OBJS)
	@ar rc $@ $^
	@echo ---------make success libftprintf.a----------

.c.o :
	@$(CC) $(FLAGS) -c $< -o $(<:.c=.o) -include $(INCLUDE)

clean :
	@rm -f $(OBJS)

fclean : clean
	@rm -f $(NAME)

re : fclean all
