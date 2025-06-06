LIB_FT_DIR=./libft/
LIB_FT_PRINTF_DIR=./ft_printf/
LIB_FT=$(LIB_FT_DIR)libft.a
LIB_FT_PRINTF=$(LIB_FT_PRINTF_DIR)libftprintf.a
SRCS=./src/containers/string/string_heredoc_methods.c ./src/containers/string/string_dup.c ./src/containers/string/string.c ./src/containers/token/token_array.c ./src/containers/stack/stack.c ./src/parsing/syntax_tree.c\
	./src/environment/env.c ./src/environment/env_methods.c ./src/environment/env_methods_.c ./src/pathname/path_resolver.c ./src/logger/zenlogger.c\
	./src/execution/execute.c ./src/environment/path_methods.c ./src/containers/string/string_vector.c ./src/expansion/expand.c ./src/expansion/wildcard.c\
	./src/containers/string/string_mask.c ./src/containers/token/token_push_back.c ./src/signals/signals.c ./src/context/context.c ./src/built_ins/export.c ./src/containers/cells/cells.c\
	./src/expansion/string_expand.c ./src/environment/simple_export_tokenizer.c ./src/lexer/lex_quotes.c ./src/lexer/lexer_consumers.c ./src/lexer/lexer_helpers.c\
	./src/lexer/lexer_methods.c ./src/shell/prompt.c ./src/parsing/parser.c ./src/expansion/expansion_lex.c ./src/expansion/field_split.c ./src/parsing/parse_redirections.c\
	./src/containers/cells/cells_manipulation.c ./src/containers/stack/stack_manipulation.c ./src/containers/string/string_manipulation.c ./src/context/context_setters.c ./src/context/context_getters.c ./src/context/context_utils.c\
	./src/containers/string/string_vconstructors.c ./src/containers/string/string_subs.c ./src/containers/token/token_array_manipulation.c ./src/parsing/parse_command.c ./src/execution/redirections.c\
	./src/containers/string/string_mask_manipulation.c  ./src/built_ins/cd.c ./src/built_ins/echo.c ./src/built_ins/env_command.c ./src/parsing/parse_subshell.c ./src/execution/execute_command.c\
	./src/built_ins/exit.c ./src/built_ins/pwd.c ./src/built_ins/unset.c ./src/signals/here_doc_signals.c ./src/parsing/heredoc_utils.c ./src/parsing/setup_heredoc.c ./src/parsing/parse_and_or_pipe.c\
	./src/environment/env_insert_defaults.c ./src/environment/env_join.c ./src/expansion/tokens_expand.c ./src/containers/token/tokens_copying.c ./src/execution/built_ins_exec.c\
	./src/expansion/wildcard_match.c ./src/containers/token/tokens_wild_card_helpers.c ./src/expansion/wildcard_open_dir.c ./src/expansion/wildcard_errors.c ./src/execution/execute_pipe.c\
	./src/lexer/lexer_consume_symbol.c ./src/expansion/find_next_expansion.c ./src/shell/shell.c ./src/environment/env_shlvl_handler.c ./src/execution/subshell_execute.c\
	./src/expansion/eshould_field_split.c
OBJS=$(SRCS:%.c=%.o)
ZEN_LIB=libzen.a
NAME=minishell
CC=cc -g
LIBS=-L. -L$(LIB_FT_PRINTF_DIR) -L$(LIB_FT_DIR) -lzen -lftprintf -lreadline -lft
INCLUDE=-I./include
MAIN=./src/zen.c
CFLAGS=-Wall -Wextra -Werror $(INCLUDE)
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
