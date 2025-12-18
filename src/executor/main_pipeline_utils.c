/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:13:40 by musajid           #+#    #+#             */
/*   Updated: 2025/12/18 17:03:46 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	child_process_multiple_pipeline(t_exec *exec,
			t_shell *shell, t_cmd *command)
{
	setup_child_signals();
	if (!is_builtin(command) && init_exec(exec, shell, command) != 0)
		set_the_code_and_exit(shell, exec, 127);
	if (command->redirs)
	{
		if (applying_redir(command, &(exec->fd->in_fd),
				&(exec->fd->out_fd)) == 1)
			set_the_code_and_exit(shell, exec, GENERAL_ERROR);
	}
	if (fds_manipulation_and_execution(command, shell, exec) == 1)
		set_the_code_and_exit(shell, exec, COMMAND_NOT_FOUND);
	exit(0);
}

static void	handle_init_exec_error(t_exec *exec, t_shell *shell, t_cmd *command)
{
	char	*cmd_name_for_error;

	freearray(exec->envp);
	exec->envp = NULL;
	shell->exit_code = 127;
	if (command->argv && command->argv[0] && command->argv[0][0])
		cmd_name_for_error = command->argv[0];
	else
		cmd_name_for_error = command->unexpanded_cmd;
	if (cmd_name_for_error)
	{
		ft_putstr_fd(cmd_name_for_error, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
}

int	init_exec(t_exec *exec, t_shell *shell, t_cmd *command)
{
	pre_init(exec);
	exec->envp = envp_arr(shell);
	if (!exec->envp)
		return (1);
	if (command->argv && command->argv[0])
		exec->path_to_exec = pathtoexecute(command->argv, exec);
	if (!exec->path_to_exec)
	{
		handle_init_exec_error(exec, shell, command);
		return (1);
	}
	return (0);
}

int	intialize_and_process_single_child(t_exec *exec,
	t_shell *shell, t_cmd *command)
{
	if (init_exec(exec, shell, command) == 0)
	{
		if (command->redirs)
		{
			if (applying_redir(command, &(exec->fd->in_fd),
					&(exec->fd->out_fd)) == 1)
				return (err_if_redir_fails(exec, shell));
		}
		if (child_process(command, shell, exec) != 0)
		{
			close_fd(exec->fd);
			return (1);
		}
	}
	else
		return (1);
	return (0);
}

int	initialize_and_process_multiple_child(t_exec *exec,
	t_shell *shell, t_cmd *command)
{
	if (pipe(exec->fd->fd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	setup_execution_signals();
	exec->pid = fork();
	if (exec->pid < 0)
	{
		perror("fork failed in multiple child process");
		close(exec->fd->fd[0]);
		close(exec->fd->fd[1]);
		setup_parent_signals();
		return (1);
	}
	if (exec->pid == 0)
	{
		return (child_process_multiple_pipeline(exec, shell, command));
	}
	return (0);
}
