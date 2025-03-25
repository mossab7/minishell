SRCS=ft_atoi.c ft_bzero.c ft_isalnum.c\
	ft_isalpha.c ft_isascii.c ft_isdigit.c\
	ft_isprint.c ft_memchr.c ft_memcmp.c ft_memcpy.c\
	ft_memset.c ft_tolower.c ft_toupper.c\
	ft_memmove.c ft_strlen.c ft_strchr.c ft_strdup.c\
	ft_strlcat.c ft_strlcpy.c ft_strrchr.c ft_strncmp.c\
	ft_strnstr.c ft_calloc.c ft_substr.c ft_itoa.c ft_split.c\
	ft_strjoin.c ft_strtrim.c ft_strmapi.c ft_striteri.c\
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c\
	resource_man.c ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c\
	ft_lstclear_bonus.c ft_lstdelone_bonus.c ft_lstiter_bonus.c\
	ft_lstlast_bonus.c ft_lstmap_bonus.c ft_lstnew_bonus.c\
	ft_lstsize_bonus.c ft_strcmp.c ft_realloc.c ft_free.c ft_malloc.c\
	ft_ispunct.c ft_isspace.c ft_isquote.c ft_atol_base.c ft_readline.c\
	get_next_line.c get_next_line_utils.c ft_is_not_null.c ft_isdir.c
OBJS=$(SRCS:%.c=%.o)
NAME=libft.a
INCLUDE=-I../include
CC=cc
CFLAGS=-Wall -Wextra -Werror
AR=ar -rsc
RM=rm -f

all: $(NAME)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
clean:
	$(RM) $(OBJS)
$(NAME): $(OBJS)
	$(AR) $@ $?
fclean: clean
	$(RM) $(NAME)
re: fclean all
.PHONY: re fclean objs clean all
