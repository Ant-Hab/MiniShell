#include "execution.h"

/* 

==1==we will check whether it is a builtin or binary
==2==separate pipelines for both
==3==if not builtin then child process
==4==before creating the child process we will see if we have pipe or redirections
==5==also need to handle a case where binary and builtinn both commands come

buitin commands
echo
pwd
cd
env
export
unset
exit

1- builtin --no fork
2- redirection + echo or bin  -- fork
3- pipe + redirection + builtin --fork

int dup2(int oldfd, int newfd);
  dup2()
       The dup2() system call performs the same task as dup(), but instead of using the lowest-numbered  unused  file
       descriptor,  it uses the file descriptor number specified in newfd.  In other words, the file descriptor newfd
       is adjusted so that it now refers to the same open file description as oldfd.

       If the file descriptor newfd was previously open, it is closed before being reused;  the  close  is  performed
       silently (i.e., any errors during the close are not reported by dup2()).

       The  steps  of closing and reusing the file descriptor newfd are performed atomically.  This is important, be‐
       cause trying to implement equivalent functionality using close(2) and dup() would be subject  to  race  condi‐
       tions,  whereby newfd might be reused between the two steps.  Such reuse could happen because the main program
       is interrupted by a signal handler that allocates a file descriptor, or because a parallel thread allocates  a
       file descriptor.

       Note the following points:

       •  If oldfd is not a valid file descriptor, then the call fails, and newfd is not closed.

       •  If  oldfd  is a valid file descriptor, and newfd has the same value as oldfd, then dup2() does nothing, and
          returns newfd.
	RETURN VALUE
       On  success, these system calls return the new file descriptor.  On error, -1 is returned, and errno is set to
       indicate the error.
*/

static void	pipe_execution(t_cmd	*command, t_shell *shell);

// void	execution(char **cmd, char **env);

void waitstatus(pid_t pid,  t_shell *shell);

void	execution_pipeline(t_cmd *command, t_shell *shell)
{
	t_cmd	*t_command;
	int		i;
	int		pipe_number;
	
	i = 0;
	t_command = command;
    while(t_command)
	{
		if (t_command->argv)
			i++;
		t_command = t_command->next;
	}
	pipe_number = i - 1;
	if (!pipe_number && !(command->redirs) && !is_builtin(command))//if there is no pipe and cmd is binary
		child_process(command, shell);
	else	
		pipe_execution(command, shell);
	
}

static void init_fd(t_fd	*fd)
{
	fd->fd[0] = -1;
	fd->fd[1] = -1;
	fd->prev_fd = -1;
	fd->in_fd = -1;
	fd->out_fd = -1;
}

static void close_fd(t_fd *fd)
{
	if (fd->fd[0] >= -1)
		close(fd->fd[0]);
	if (fd->fd[1] >= -1)
		close(fd->fd[1]);
	if (fd->prev_fd >= -1)
		close(fd->prev_fd);
	if (fd->in_fd >= -1)
		close(fd->in_fd);
	if (fd->out_fd >= -1)
		close(fd->out_fd);
}

static void parent_loop(t_cmd *cmd, t_fd *fd)
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

static void	execute_pipe(t_cmd *cmd, t_fd *fd, char **arr)
{
	if (cmd->redirs)
		applying_redir(cmd->redirs, &(fd->in_fd), &(fd->out_fd));
	if (fd->prev_fd != -1)
		dup2(fd->prev_fd, STDIN_FILENO);
	if (fd->in_fd != -1)
		dup2(fd->in_fd, STDIN_FILENO);
	if (cmd->next != NULL && fd->out_fd == -1)
		dup2(fd->fd[1], STDOUT_FILENO);
	if (fd->out_fd != -1)
		dup2(fd->out_fd, STDOUT_FILENO);
	close_fd(fd);
	execution(cmd->argv, arr);
}

static void pipe_execution(t_cmd *command, t_shell *shell)
{
    pid_t pid;
    t_fd fd;

    char **envp = envp_arr(shell);
    init_fd(&fd);
    while (command)
    {
        pipe(fd.fd);
        pid = fork();
		if (pid < 0)
			perror("fork failed in child process");
        if (pid == 0)
			execute_pipe(command, &fd, envp);
		parent_loop(command, &fd);
        command = command->next;
    }
	// waitstatus(pid, shell);
    freearray(envp);
}
void waitstatus(pid_t pid,  t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if WIFEXITED(status)
		shell->exit_code = WEXITSTATUS(status);
}