LIB_FT_DIR=./libft/
LIB_FT_PRINTF_DIR=./ft_printf/
LIB_FT=$(LIB_FT_DIR)libft.a
LIB_FT_PRINTF=$(LIB_FT_PRINTF_DIR)libftprintf.a
SRCS=./src/lexer/lexer.c ./src/data_structures/string.c ./src/data_structures/token_array.c ./src/data_structures/stack.c ./src/parsing/syntax_tree.c\
	./src/environment/env.c ./src/environment/env_methods.c ./src/environment/built_in_commands.c ./src/pathname/path_resolver.c ./src/logger/zenlogger.c\
	 ./src/execution/execute.c ./src/environment/path_methods.c ./src/data_structures/string_vector.c ./src/expansion/expand.c ./src/wildcard.c\
	./src/data_structures/string_mask.c ./src/data_structures/token_push_back.c ./src/signals/signals.c
OBJS=$(SRCS:%.c=%.o)
ZEN_LIB=libzen.a
NAME=minishell
CC=cc
LIBS=-L. -L$(LIB_FT_PRINTF_DIR) -L$(LIB_FT_DIR) -lzen -lftprintf -lreadline -lft -ggdb
INCLUDE=-I./include
MAIN=./src/zen.c
CFLAGS=-Wall -Wextra -Werror $(INCLUDE) -ggdb
AR=ar rsc
RM=rm -f

all: $(NAME)
%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
$(ZEN_LIB): $(LIB_FT_PRINTF) $(LIB_FT) $(OBJS)
	$(AR) $@ $?
$(LIB_FT_PRINTF):
	make -C $(LIB_FT_PRINTF_DIR)
$(LIB_FT):
	make -C $(LIB_FT_DIR)
$(NAME):  $(ZEN_LIB) $(MAIN)
	$(CC) $(CFLAGS) $(MAIN) -o $@ $(LIBS)
$(BONUS): $(PUSH_SWAP_LIB)
	$(CC) $(CFLAGS) ./bonus/main.c -o $@ $(LIBS)
clean:
	make -C $(LIB_FT_PRINTF_DIR) clean
	make -C $(LIB_FT_DIR) clean
	$(RM) $(OBJS)
	$(RM) $(ZEN_LIB)
fclean: clean
	make -C $(LIB_FT_PRINTF_DIR) fclean
	make -C $(LIB_FT_DIR) fclean
	$(RM) $(NAME)
re: fclean all
.PHONY: re fclean clean bonus
