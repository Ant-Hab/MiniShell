#include "minishell.h"

/* Check if a token is a pipe '|' */
int	is_pipe_token(const char *token_str)
{
	// returns 1 when token_str equals "|"
	return (token_str && ft_strcmp(token_str, "|") == 0);
}

/* Check if a token is a redirection: <, >, >>, << */
int	is_redir_token(const char *token_str)
{
	// returns 1 for any redirection operator
	return (token_str && (!ft_strcmp(token_str, "<") || !ft_strcmp(token_str, ">")
			|| !ft_strcmp(token_str, ">>") || !ft_strcmp(token_str, "<<")));
}

/* Convert a redirection token string to t_redir_type enum */
t_redir_type	get_redir_type(const char *token_str)
{
	// map token to enum type
	if (!ft_strcmp(token_str, "<"))
		return (R_INPUT);
	if (!ft_strcmp(token_str, ">"))
		return (R_OUTPUT);
	if (!ft_strcmp(token_str, ">>"))
		return (R_APPEND);
	return (R_HEREDOC);
}
