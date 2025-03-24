#include <zen.h>

void	print_ast(t_ast *node, int level)
{
	t_redirect	*redir;

	if (node == NULL)
	{
		return ;
	}
	for (int i = 0; i < level; i++)
	{
		printf("    ");
	}
	switch (node->type)
	{
	case NODE_COMMAND:
	{
		printf("├── COMMAND\n");
		for (int i = 0; i < level + 1; i++)
		{
			printf("    ");
		}
		printf("├── ARGS: ");
		for (int i = 0; i < node->value.command.argc; i++)
		{
			printf("%s ", node->value.command.args[i]);
		}
		printf("\n");
		for (int i = 0; i < level + 1; i++)
		{
			printf("    ");
		}
		printf("├── REDIRECTS: ");
		for (int i = 0; i < node->value.command.redirect_count; i++)
		{
			redir = node->value.command.redirects[i];
			if (redir->type == REDIR_INPUT)
			{
				printf("< %s ", redir->filename);
			}
			else if (redir->type == REDIR_OUTPUT)
			{
				printf("> %s ", redir->filename);
			}
			else if (redir->type == REDIR_APPEND)
			{
				printf(">> %s ", redir->filename);
			}
			else if (redir->type == REDIR_HEREDOC)
			{
				printf("<< %s ", redir->delimiter);
			}
		}
		printf("\n");
		break ;
	}
	case NODE_PIPE:
		printf("├── PIPE\n");
		break ;
	case NODE_SUBSHELL:
		printf("├── SUBSHELL\n");
		break ;
	case NODE_LOGICAL_AND:
		printf("├── AND\n");
		break ;
	case NODE_LOGICAL_OR:
		printf("├── OR\n");
		break ;
	default:
		printf("├── UNKNOWN NODE TYPE\n");
		break ;
	}
	if (node->left)
	{
		print_ast(node->left, level + 1);
	}
	if (node->right)
	{
		print_ast(node->right, level + 1);
	}
}
