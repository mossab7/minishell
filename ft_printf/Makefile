SRCS=ft_math.c ft_chop.c ft_iprint.c ft_isdigit.c ft_length.c ft_memset.c ft_parse.c ft_printf.c ft_putu.c ft_spadding.c ft_sprint.c ft_vprintf_intern.c ft_xprint.c ft_pprint.c ft_cprint.c ft_parse_int.c ft_io.c state_managers.c
OBJS=$(SRCS:%.c=%.o)
INCLUDE=-I../include
NAME=libftprintf.a
CC=cc
CFLAGS=-Wall -Wextra -Werror
AR=ar rsc
RM=rm -f

all: $(NAME)
%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(INCLUDE)
clean:
	$(RM) $(OBJS)
$(NAME): $(OBJS)
	$(AR) $@ $?
fclean: clean
	$(RM) $(NAME)
re: fclean all
.PHONY: re fclean clean bonus
