#include "minishell.h"

void waitstatus(pid_t pid,  t_shell *shell);

void init_fd(t_fd	*fd)
{
	fd->fd[0] = -1;
	fd->fd[1] = -1;
	fd->prev_fd = -1;
	fd->in_fd = -1;
	fd->out_fd = -1;
}

void close_fd(t_fd *fd)
{
	if (fd->fd[0] >= 0)
		close(fd->fd[0]);
	if (fd->fd[1] >= 0)
		close(fd->fd[1]);
	if (fd->prev_fd >= 0)
		close(fd->prev_fd);
	if (fd->in_fd >= 0)
		close(fd->in_fd);
	if (fd->out_fd >= 0)
		close(fd->out_fd);
	init_fd(fd);
}

void parent_loop(t_cmd *cmd, t_fd *fd)
{
	//parent closes the prev
	if (fd->prev_fd != -1)
            close(fd->prev_fd);

        // Keep current read end for next command
    if (cmd->next != NULL)
        fd->prev_fd = fd->fd[0];
    else
        fd->prev_fd = -1;
        // Close write end in parent
    close(fd->fd[1]);
}


int	fds_manipulation_and_execution(t_cmd *command, t_shell *shell, t_exec *exec)
{
	//if there is pipe, if not then go to the execution
	if (exec->fd->prev_fd != -1)
		dup2(exec->fd->prev_fd, STDIN_FILENO);
	if (exec->fd->in_fd != -1)
		dup2(exec->fd->in_fd, STDIN_FILENO);
	if (command->next != NULL && exec->fd->out_fd == -1)
		dup2(exec->fd->fd[1], STDOUT_FILENO);
	if (exec->fd->out_fd != -1)
		dup2(exec->fd->out_fd, STDOUT_FILENO);
	close_fd(exec->fd);
	if (execution(command, shell, exec) == 1)
		exit_after_execve(shell, exec);
	free(exec->path_to_exec);
	if (exec->envp != NULL)
		freearray(exec->envp);
	execution_cleanup(shell);
	exit(shell->exit_code);
}

void main_pipeline(t_shell *shell, t_cmd *command)
{
	t_exec	*exec;

	exec = shell->exec;
	if (is_parent_level_builtin(command) && !command->next)
	{
		if (command->redirs)
		{
			if (applying_redir(command, &shell->exec->fd->in_fd, &shell->exec->fd->out_fd) == 1)
			{
				shell->exit_code = GENERAL_ERROR;
				return ;
			}
		}
		shell->exit_code = run_builtin(command, shell);
		/* Close any opened redir fds and the shell->exec->fd structure fds */
		if (shell->exec->fd->in_fd != -1)
		{
			close(shell->exec->fd->in_fd);
			shell->exec->fd->in_fd = -1;
		}
		if (shell->exec->fd->out_fd != -1)
		{
			close(shell->exec->fd->out_fd);
			shell->exec->fd->out_fd = -1;
		}
		close_fd(shell->exec->fd);
		return;
	}
		// single or piped command execution path
	validate_command(exec, shell, command);
	return ;
}

void waitstatus(pid_t pid,  t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
    	shell->exit_code = WEXITSTATUS(status);
}

