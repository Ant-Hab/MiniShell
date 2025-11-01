#include "minishell.h"

/* 
 * Print a single redirection list
 * - Prints type (<, >, >>, <<) and target
 * - No indentation
 */
static void	print_redir(t_redir *r)
{
	char	*type_str;                             // redir symbol
	char	*target;                               // redir target filename

	while (r)
	{
		if (r->type == R_INPUT)
			type_str = "<";
		else if (r->type == R_OUTPUT)
			type_str = ">";
		else if (r->type == R_APPEND)
			type_str = ">>";
		else
			type_str = "<<";

		if (r->target)
			target = r->target;
		else
			target = "(null)";

		ft_printf("REDIR: %s -> %s\n", type_str, target); // print type and target
		r = r->next;                                     // move to next redirection
	}
}

/* 
 * Print all tokens for debugging
 * - Shows token index, string content, and type
 * - Adds blank lines before and after section
 */
void	dbg_print_tokens(t_list *tokens)
{
	int		i;                                      // token index
	t_token *tok;                                  // current token
	char	*type_str;                             // string representation of type

	ft_printf("\n---- DEBUG TOKENS ----\n");       // blank line before
	i = 0;
	while (tokens)
	{
		tok = tokens->content;                     // cast content to token
		if (tok && tok->str)
		{
			if (tok->type == T_WORD)
				type_str = "WORD";
			else if (tok->type == T_QUOTE)
				type_str = "QUOTE";
			else
				type_str = "OPERATOR";

			ft_printf("[%d] \"%s\" (%s)\n", i, tok->str, type_str); // print token
		}
		else
			ft_printf("[%d] (null)\n", i);         // null token

		i++;                                        // next index
		tokens = tokens->next;                     // move to next token
	}
	ft_printf("----------------------\n\n");    // longer blank line after
}

/* 
 * Print all commands for debugging
 * - Shows command index, argv contents, and redirections
 * - Redirections are not indented
 * - Adds blank lines before and after section
 */
void	dbg_print_cmds(t_cmd *cmd)
{
	int	i;                                          // command index
	int	j;                                          // argv index

	ft_printf("\n---- DEBUG COMMANDS ----\n");       // blank line before
	i = 0;
	while (cmd)
	{
		ft_printf("CMD[%d]:", i);                   // print command index
		if (!cmd->argv || !cmd->argv[0])
			ft_printf(" (empty)\n");               // empty argv
		else
		{
			j = 0;
			while (cmd->argv[j])
			{
				ft_printf(" '%s'", cmd->argv[j]); // print argv element
				j++;                               // next argv
			}
			ft_printf("\n");                       // newline after argv
		}

		print_redir(cmd->redirs);                    // print redirections (no indent)
		i++;                                         // next command
		cmd = cmd->next;                             // move to next
	}
	ft_printf("------------------------\n\n"); // longer line after
}

/* 
 * Print last exit code for debugging
 * - Adds blank lines before and after section
 */
void	dbg_print_exit_code(int code)
{
	ft_printf("\n---- DEBUG EXIT CODE ----\n");     // blank line before
	ft_printf("Exit code: %d\n", code);             // print exit code
	ft_printf("-------------------------\n\n");  // longer line after
}
