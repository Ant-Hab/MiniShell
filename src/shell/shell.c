#include "minishell.h"

int  init_shell_and_arena(t_shell *shell, t_arena **arena, char **envp)
{
    shell->env = NULL;
    shell->exit_code = 0;
    shell->running = true;

    *arena = init_arena(1024);
    if (!*arena)
    {
        ft_printf("minishell: failed to initialize memory arena\n");
        return (1);
    }
    shell->env = init_env(envp);
    if (!shell->env)
    {
        ft_printf("minishell: failed to initialize environment\n");
        free_arena(arena);
        return (1);
    }
    shell->exec = ft_calloc(1, sizeof(t_exec));
    if (!shell->exec)
    {
        free_env(shell->env);
        free_arena(arena);
        return (1);
    }
    shell->exec->fd = ft_calloc(1, sizeof(t_fd));
    if (!shell->exec->fd)
    {
        free(shell->exec);
        free_env(shell->env);
        free_arena(arena);
        return (1);
    }
    init_fd(shell->exec->fd);
    return (0);
}
