/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 17:20:39 by musajid           #+#    #+#             */
/*   Updated: 2025/12/18 17:20:40 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		err;

	(void)argc;
	(void)argv;
	ft_bzero(&shell, sizeof(t_shell));
	err = init_shell_and_arena(&shell, &shell.arena, envp);
	if (err)
		return (1);
	setup_parent_signals();
	if (isatty(STDIN_FILENO))
		repl_loop(&shell, &shell.arena);
	else
		non_interactive_loop(&shell, &shell.arena);
	shell_cleanup(&shell);
	return (shell.exit_code);
}
