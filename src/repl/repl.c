#include "minishell.h"

/* 
 * Main REPL loop
 * - Reads input from user
 * - Tokenizes input using arena
 * - Expands variables (currently disabled)
 * - Parses commands using arena
 * - Executes commands (currently disabled)
 * - Loops until shell->running == false
 */
void	repl_loop(t_shell *shell, t_arena **arena)
{
	char    *input;     // input line (arena memory)
	t_list  *tokens;    // token list (arena nodes)
	t_cmd   *commands;  // parsed commands (arena nodes)

	while (shell->running)
	{
		// printf("before anything");
		input = read_input(arena);               // read user input into arena
	
		if (!input)
			break;                               // Ctrl-D exits
	
		tokens = tokenize(input, arena);       // tokenize input using arena
		if (!tokens)
			break;
		// dbg_print_tokens(tokens);                // debug tokens


		/* parse tokens into commands */
	
		commands = parse_tokens(tokens, arena);  // build command structures
		
		if (!commands && tokens)                 // parse failed (syntax or alloc)
		{
			// parsing error already printed by parse_tokens()
			(void)tokens;
			continue;                             // skip execution
		}
		// dbg_print_cmds(commands);                // show parsed commands

		/* tests for built-in commands (for now) */
		//test_builtin(commands, shell);
		
		/* execute commands */
		if (commands->argv != NULL)
			execution_pipeline(commands, shell);
		dbg_print_exit_code(shell->exit_code);
		// dbg_print_exit_code(shell->exit_code);   // debug exit code
		(void)tokens;
		(void)commands;
	}
}

// typedef enum e_redir_type
// {
// 	R_INPUT,
// 	R_OUTPUT,
// 	R_APPEND,
// 	R_HEREDOC
// }	t_redir_type;

// typedef struct s_redir
// {
// 	t_redir_type	type;   /* type of redirection */
// 	char			*target;/* file or heredoc target */
// 	struct s_redir	*next;  /* next redirection */
// }	t_redir;

// typedef struct s_cmd
// {
// 	char			**argv;  /* argument vector */
// 	t_redir			*redirs; /* redirections */
// 	struct s_cmd	*next;   /* next command */
// }	t_cmd;

// if (cmd->redir->type)
// 	{
// 		if(cmd->redir->type == in)
// 			{//check access
// 			int open_fd = open(cmd->redir->target, O_RDONLY);
// 			dup2(fd[0], open_fd);
// 			execution_pipeline
// 		}
// 	}
// have to handle redire for built in as well 