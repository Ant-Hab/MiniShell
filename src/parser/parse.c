#include "minishell.h"

/* Create and append a new command node to the list
 * - Uses arena allocation
 * - Returns pointer to head or NULL on failure
 */
static t_cmd	*append_new_cmd(t_cmd **head, t_arena **arena)
{
	t_cmd	*node;
	t_cmd	*iter;

	node = create_cmd_node(arena);		// Allocate new command node from arena
	if (!node)
		return (NULL);
	if (!*head)
	{
		*head = node;					// If list is empty, set new node as head
		return (*head);
	}
	iter = *head;
	while (iter->next)					// Traverse to end of list
		iter = iter->next;
	iter->next = node;					// Append new node to end
	return (*head);
}

/* Handle word or quoted token
 * - Creates command if missing
 * - Adds token->str to argv
 * - Returns 1 on success, 0 on failure
 */
static int	handle_word_token(t_cmd **cur, t_cmd **head, t_token *tok, t_arena **arena)
{
	if (!*cur)							// If no current command exists, create one
	{
		if (!append_new_cmd(head, arena))
			return (0);
		*cur = *head;
		while ((*cur)->next)			// Traverse to the last command (newly created one)
			*cur = (*cur)->next;
	}
	// add word string to argv
	if (!add_word_to_argv(*cur, tok->str, arena))
		return (0);
	return (1);
}

/* Handle operator tokens
 * - Pipes create a new command slot
 * - Redirs require next token as filename
 * Returns: 1 success, -1 syntax error, 0 alloc fail
 */
static int	handle_operator_token(t_list **tokens_ref, t_cmd **cur, t_cmd **head, t_arena **arena)
{
	t_token			*op_tok;
	t_list			*next_node;
	t_token			*next_tok;
	t_redir_type	rtype;

	op_tok = (*tokens_ref)->content;
	if (is_pipe_token(op_tok->str))		// Handle pipe operator
	{
		if (!*cur || !(*cur)->argv || !(*cur)->argv[0])	// Check for pipe without preceding command
			return (-1);
		*cur = NULL;					// Reset current command pointer for next command
		return (1);
	}
	if (is_redir_token(op_tok->str))	// Handle redirection operators
	{
		next_node = (*tokens_ref)->next;
		if (!next_node)					// Get next token which should be the filename
			return (-1);
		next_tok = next_node->content;
		if (!next_tok || !next_tok->str)
			return (-1);
		if (!*cur)						// Ensure command exists before adding redirection
		{
			if (!append_new_cmd(head, arena))
				return (0);
			*cur = *head;
			while ((*cur)->next)
				*cur = (*cur)->next;
		}
		rtype = get_redir_type(op_tok->str);
		if (!add_redirection(*cur, rtype, next_tok->str, arena))	// Add redirection to command
			return (0);
		*tokens_ref = next_node;		// Skip filename token since we've processed it
		return (1);
	}
	return (0);
}

/* Parse token list into command list
 * - Handles pipes, words, and redirs
 * - Returns head or NULL on failure
 */
t_cmd	*parse_tokens(t_list *tokens, t_arena **arena)
{
	t_cmd	*head;
	t_cmd	*cur;
	t_list	*it;
	t_token	*tok;
	int		res;

	head = NULL;
	cur = NULL;
	it = tokens;
	while (it)							// Iterate through all tokens
	{
		tok = it->content;
		if (!tok)
			return (NULL);
		if (tok->type == T_WORD || tok->type == T_QUOTE)	// Handle word and quoted tokens
		{
			// normal word or quoted string
			if (!handle_word_token(&cur, &head, tok, arena))	// Process normal word or quoted string
				return (ft_printf("minishell: parse error: alloc fail\n"), NULL);
		}
		else
		{
			res = handle_operator_token(&it, &cur, &head, arena);	// Process operator tokens
			if (res == -1)
				return (ft_printf("minishell: syntax error near '%s'\n", tok->str), NULL);
			if (res == 0)
				return (ft_printf("minishell: parse error: alloc fail\n"), NULL);
		}
		it = it->next;
	}
	return (head);
}
