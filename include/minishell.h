/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mumar <mumar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 16:46:44 by mumar             #+#    #+#             */
/*   Updated: 2024/05/18 16:51:56 by mumar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/stat.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include "libft.h"
# include "minishell_types.h"

# define BUFFER_SIZE_FOR_ARENA 1024
# define FIELD_SEP 0x1F

void	*arena_alloc(t_arena **arena, size_t size);
void	*arena_realloc(t_arena **arena, void *old_ptr, size_t old_size,
			size_t new_size);
t_arena	*init_arena(size_t size);
t_arena	*new_bigger_arena(t_arena *current_arena, size_t size);
char	*arena_strdup(t_arena **arena, const char *s);
char	*arena_strndup(t_arena **arena, const char *s, size_t n);
void	free_arena(t_arena **arena);
void	arena_clear(t_arena **arena);

int		main(int argc, char **argv, char **envp);
int		init_shell_and_arena(t_shell *shell, t_arena **arena, char **envp);
void	shell_cleanup(t_shell *shell);
void	repl_loop(t_shell *shell, t_arena **arena);
void	non_interactive_loop(t_shell *shell, t_arena **arena);
char	*read_input(t_arena **arena);
char	*read_heredoc(t_shell *shell, const char *delimiter);
char	*handle_heredoc(t_cmd *cmd, t_shell *shell, const char *delimiter);
void	print_hd_err(const char *delimiter);

void	setup_parent_signals(void);
void	setup_execution_signals(void);
void	setup_child_signals(void);
void	setup_hd_signals(void);
void	handle_hd_signal(int sig);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	exec_sigint(int sig);
void	exec_sigquit(int sig);

t_env	*init_env(char **envp);
void	free_env(t_env *env);
int		add_pwd(const char *str, t_shell *shell, char *oldpwd);

t_list	*tokenize(char *input, t_arena **arena);
int		skip_spaces(char *s, int i);
int		skip_quote_chunk(char *s, int i);
int		is_operator_char(char c);
int		create_token_node(t_arena **ar, t_list **toks,
			char *content, t_token_type type);
char	*dup_word(t_arena **arena, const char *str, int start, int end);
int		handle_operator(char *s, int i, t_list **tokens, t_arena **arena);
int		handle_quote(char *s, int i, t_list **tokens, t_arena **arena);
int		handle_word(char *s, int i, t_list **tokens, t_arena **arena);
void	free_tokens(t_list **tokens);

t_cmd	*parse_tokens(t_list *tokens, t_shell *shell, t_arena **arena);
t_cmd	*create_cmd_node(t_arena **arena);
int		ensure_current_cmd(t_cmd **cur, t_cmd **head, t_arena **arena);
int		add_word_to_argv(t_cmd *cmd, const char *word, t_arena **arena);
int		add_redirection(t_cmd *cmd, t_redir_type type, const char *target,
			t_arena **arena);
int		handle_redir_token(t_list **tokens_ref, t_cmd **cur,
			t_cmd **head, t_shell *shell);
int		is_pipe_token(const char *tok);
int		is_redir_token(const char *tok);
t_redir_type	get_redir_type(const char *tok);

char	*expand_string(const char *str, t_shell *shell, t_arena **arena);
int		expand_command_argv(t_cmd *cmd, t_shell *shell, t_arena **arena);
char	*append_char(char *buf, size_t *len, char c, t_arena **arena);
char	*expand_env_value(const char *key, t_shell *shell, t_arena **arena);
char	*expand_variable(const char *str, size_t *i, t_shell *shell,
			t_arena **arena);
char	**field_split(const char *str, t_arena **arena);

int		is_builtin(t_cmd *cmd);
int		run_builtin(t_cmd *cmds, t_shell *shell, bool is_child_process);
long	ft_atol(const char *s, int *error);
int		ft_env(t_env *head);
int		ft_echo(char **av);
int		ft_pwd(void);
int		ft_exit(char **av, t_shell *shell, t_arena **arena,
			bool is_child_process);
int		ft_unset(t_cmd *cmd, t_shell *shell);
int		ft_export(t_cmd *cmds, t_shell *shell);
int		ft_cd(t_cmd *cmds, t_shell *shell);
void	update_cd_node(char *value, t_env *node);
t_env	*find_env_node(const char *str, t_env *head);
int		get_env_length(t_env *head);
t_env	**alloc_assign(int len, t_env *head);
void	sort_env(t_env ***arr);
int		update_env_node(const char *str, t_shell *shell);
int		add_env_node(const char *str, t_shell *shell);
void	print_export(t_env **arr);
int		ft_exit2(char **av, t_shell *shell);
void	free_env_node(t_env *node);

t_cmd	*parse_error(const char *msg, const char *tok);
int		count_argv(char **argv);
int		iterate_key(const char *str);
int		is_blank_line(const char *s);

void	dbg_print_tokens(t_list *tokens);
void	dbg_print_cmds(t_cmd *cmds);
void	dbg_print_exit_code(int exit_code);
void	dbg_print_expanded_argv(t_cmd *cmd);

# define COMMAND_NOT_FOUND 127
# define PERMISSION_DENIED 126
# define EXECUTION_SUCESS 0
# define GENERAL_ERROR 1
# define ENV_PATH_COULDNT_BE_FOUND 2
# define ENVIRONMENT_COPY_FAILED 3
# define FORK_FAILED 4

void	execution_cleanup(t_shell *shell);
void	set_the_code_and_exit(t_shell *shell, t_exec *exec, int type);
void	exit_after_execve(t_shell *shell, t_exec *exec);

char	**get_path(char **envp_arr);
char	*search_path_for_cmd(char *cmd_name, char **paths);
int		path_is_set(char **envp);
char	*pathtoexecute(char **cmd, t_exec *exec);
void	checking(char *path, char *cmd);
int		child_process(t_cmd *cmd, t_shell *shell, t_exec *exec);
char	**envp_arr(t_shell *shell);

void	strerrornexit(void);
void	freearray(char **arr);
void	freestrnarrexit(char **arr, char *str, int i);
void	freeerror(char **arr);
void	freeall(char **arr, char *str, char *cmd);
void	commandnotfound(char **arr);

int		init_shell(t_shell *shell, char **envp, t_arena **arena);

void	init_fd(t_fd	*fd);
void	parent_loop(t_cmd *cmd, t_fd *fd);
void	main_pipeline(t_shell *shell, t_cmd *command);
void	close_fd(t_fd *fd);
int		fds_manipulation_and_execution(t_cmd *cmd, t_shell *shell,
			t_exec *exec);
void	cleanup_pipeline(t_shell *shell, char **envp, pid_t last_pid);
int		execution(t_cmd *command, t_shell *shell, t_exec *exec);

void	pre_init(t_exec *exec);
void	clean_exec(t_exec *exec);

int		init_exec(t_exec *exec, t_shell *shell, t_cmd *command);
int		is_parent_level_builtin(t_cmd *cmd);
int		intialize_and_process_single_child(t_exec *exec, t_shell *shell,
			t_cmd *command);
int		initialize_and_process_multiple_child(t_exec *exec, t_shell *shell,
			t_cmd *cmd);
void	validate_command(t_exec *exec, t_shell *shell, t_cmd *command);

void	set_the_exit_code(t_shell *shell, char *command, char **envp);
int		applying_redir(t_cmd *cmd, int *in_fd, int *out_fd);
void	waitstatus(pid_t pid, t_shell *shell);

int		err_if_redir_fails(t_exec *exec, t_shell *shell);
void	err_if_redir_fails_parent(t_exec *exec, t_shell *shell);
void	clean_close(t_exec *exec);

#endif