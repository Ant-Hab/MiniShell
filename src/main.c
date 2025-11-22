#include "minishell.h"

/* 
 * Entry point of minishell
 * - Initializes shell state, arena, and environment
 * - Sets up signal handlers
 * - Starts the REPL loop
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	t_arena		*arena;
	(void)argc;
	(void)argv;

	ft_bzero(&shell, sizeof(t_shell));
	init_shell(&shell, envp, &arena); //env and arena initializaion
	setup_signals();
	/* start REPL loop */
	//  printf("hello");
	repl_loop(&shell, &arena);
	/* debug: print final exit code */
	//dbg_print_exit_code(shell.exit_code);
	/* cleanup */
	printf("testing");
	general_cleanup(&shell, &arena);	
	return (shell.exit_code);  // return shell exit code
}
