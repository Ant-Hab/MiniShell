#include "execution.h"

static  char    **get_path(char **envp_arr)
{
	int             i;
	char    **paths;

	i = 0;
	if (!envp_arr[i])
			return (NULL);
	while (envp_arr[i])
	{
		if (strncmp(envp_arr[i], "PATH=", 5) == 0)
		{
			paths = ft_split((envp_arr[i] + 5), ':');
			if (!paths || !*paths)
					freeerror(paths);
			return (paths);
		}
		i++;
	}
	return (NULL);
}

static	char	*join_and_check(char *dir, char *pathcmd, char **paths)
{
	char	*path;

	path = ft_strjoin(dir, pathcmd);
	if (!path)
		freestrnarrexit(paths, pathcmd, 1);
	if (access(path, X_OK) == 0)
	{
		freearray(paths);
		free(pathcmd);
		return (path);
	}
	free(path);
	return (NULL);
}

static	char	*pathtoexecute(char **cmd, char **env)
{
	int		i;
	char	**paths;
	char	*pathcmd;
	char	*path;

	pathcmd = ft_strjoin("/", cmd[0]);
	if (!pathcmd)
		strerrornexit();
	paths = get_path(env);
	if (!paths || !*paths)
		freestrnarrexit(paths, pathcmd, 127);
	i = 0;
	while (paths[i])
	{
		path = join_and_check(paths[i], pathcmd, paths);
		if (path)
			return (path);
		i++;
	}
	freeall(paths, pathcmd, cmd[0]);
	return (NULL);
}

static	void	checking(char *path)
{
	if ((access(path, F_OK) == 0) && (access(path, X_OK) == -1))
		errno = EACCES;
	if (access(path, X_OK) == 0)
		return ;
	if (access(path, X_OK))
		errno = ENOENT;
}
//need serparate function for absolute path
static void abs_path_execution(char **cmd, char **env)
{
    checking(cmd[0]);
    if (execve(cmd[0], cmd, env) == -1)
    {
        freearray(cmd);
        freearray(env);
        strerrornexit();
    }
}

static void	notfound()
{
	// void(arr);
	// freearray(arr);
	errno = ENOENT;
	// shell->exit_code = 127;

	exit(EXIT_FAILURE);
}

static void relative_path_execution(char **cmd, char **env)
{
    char *path;

	
    if (!env)
    {
        printf("copying environment failed");
        exit(EXIT_FAILURE);
    }
    if (!cmd)
        strerrornexit();
    path = pathtoexecute(cmd, env);
    if (path == NULL)
        notfound();
    checking(path);
    if (execve(path, cmd, env) == -1)
    {
        freearray(cmd);
        free(path);
        freearray(env);
        strerrornexit();
    }
}


void	execution(char **cmd, char **env)
{
	if (ft_strchr(cmd[0], '/' ))
		abs_path_execution(cmd, env);
	else
		relative_path_execution(cmd, env);
}

void	child_process(t_cmd *parsed_cmd, t_shell *shell)
{
	pid_t	pid;
	int status;
	
	
	char **envp = envp_arr(shell);
	
	pid = fork();
	if (pid == 0)
	{
		execution(parsed_cmd->argv, envp);
		perror("Minishell$ ");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		freearray(envp);
	}
	else
		perror("fork");
// waitstatus(pid, shell);
}