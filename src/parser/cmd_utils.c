#include "minishell.h"

/* Allocate a new command node from arena
 * - Initializes argv and redirection list to NULL
 */
t_cmd	*create_cmd_node(t_arena **arena)
{
	t_cmd	*cmd;                                // new command node

	cmd = arena_alloc(arena, sizeof(t_cmd));    // allocate in arena
	if (!cmd)
		return (NULL);                         // allocation failed
	cmd->argv = NULL;                           // initialize argv
	cmd->redirs = NULL;                         // initialize redirection list
	cmd->next = NULL;                           // initialize next pointer
	return (cmd);                               // return node
}

/* Count number of entries in argv array
 * - Returns count (0 if argv NULL)
 */
static int	count_argv(char **argv)
{
	int	i;                                    // index

	i = 0;
	while (argv && argv[i])                     // loop until NULL
		i++;
	return (i);                                 // return count
}

/* Add a word to the command's argv array using arena
 * - Returns 1 on success, 0 on failure
 */
int	add_word_to_argv(t_cmd *cmd, const char *word, t_arena **arena)
{
	char	**new_argv;                          // new argv array
	int		i;                                   // index
	int		count;                               // current argv size
	size_t	len;                                 // word length

	if (!cmd || !word)
		return (0);                             // invalid input
	count = count_argv(cmd->argv);              // count existing entries
	new_argv = arena_alloc(arena, sizeof(char *) * (count + 2)); /* +1 word, +1 NULL */
	if (!new_argv)
		return (0);                             // allocation failed

	i = 0;
	while (i < count)
	{
		new_argv[i] = cmd->argv[i];             // copy existing pointers
		i++;
	}

	len = ft_strlen(word);                         // compute length
	new_argv[i] = arena_alloc(arena, len + 1); /* allocate string in arena */
	if (!new_argv[i])
		return (0);                             // allocation failed
	ft_memcpy(new_argv[i], word, len + 1);         // copy including '\0'
	new_argv[i + 1] = NULL;                     // terminate array

	cmd->argv = new_argv;                       // set new argv
	return (1);                                 // success
}

/* Append a redirection to the command using arena
 * - Returns 1 on success, 0 on failure
 */
int	add_redirection(t_cmd *cmd, t_redir_type type, const char *target, t_arena **arena)
{
	t_redir	*redir;                              // new redirection node
	t_redir	*iter;                               // iterator
	size_t	tlen;                                // target length

	if (!cmd || !target)
		return (0);                             // invalid input

	redir = arena_alloc(arena, sizeof(t_redir)); /* allocate node in arena */
	if (!redir)
		return (0);                             // allocation failed
	redir->type = type;                         // set type

	tlen = ft_strlen(target);                      // compute target length
	redir->target = arena_alloc(arena, tlen + 1); /* allocate string in arena */
	if (!redir->target)
		return (0);                             // allocation failed
	ft_memcpy(redir->target, target, tlen + 1);    // copy including '\0'

	redir->next = NULL;                         // terminate node

	if (!cmd->redirs)                           // first redirection
		cmd->redirs = redir;
	else
	{
		iter = cmd->redirs;                     // iterate to last
		while (iter->next)
			iter = iter->next;
		iter->next = redir;                     // append
	}
	return (1);                                 // success
}
