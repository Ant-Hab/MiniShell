#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_shell     shell;
    int         err;

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
    /* Cleanup */
    // rl_clear_history();
    // free_env(shell.env);
    // free_arena(&shell.arena);
    // if (shell.exec && shell.exec->fd)
    // {
    //     close_fd(shell.exec->fd);
    //     free(shell.exec->fd);
    //     free(shell.exec);
    // }

    shell_cleanup(&shell);
    return (shell.exit_code);
}