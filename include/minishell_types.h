/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_types.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mumar <mumar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 16:47:04 by mumar             #+#    #+#             */
/*   Updated: 2024/05/18 16:52:16 by mumar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TYPES_H
# define MINISHELL_TYPES_H

# include <stddef.h>
# include <stdbool.h>

typedef struct s_fd
{
	int		fd[2];
	int		prev_fd;
	int		in_fd;
	int		out_fd;
}			t_fd;

typedef struct s_exec
{
	t_fd	*fd;
	pid_t	pid;
	char	*path_to_exec;
	char	**envp;
	int		exit_flag;
}			t_exec;

typedef struct s_arena
{
	void			*memory_block;
	size_t			buffer;
	size_t			offset;
	struct s_arena	*next;
}					t_arena;

# define GROWTH_FACTOR 2

typedef struct s_list	t_list;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef enum e_exec_flow
{
	FLOW_OK = 0,
	FLOW_EXIT
}	t_exec_flow;

typedef struct s_shell
{
	t_env		*env;
	t_arena		*arena;
	int			exit_code;
	bool		running;
	t_exec		*exec;
	t_exec_flow	exit_flow;
	bool		should_print_exit_message;
}				t_shell;

typedef enum e_token_type
{
	T_WORD,
	T_OPERATOR,
	T_QUOTE,
	T_HEREDOC
}	t_token_type;

typedef struct s_token
{
	char			*token;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

typedef enum e_redir_type
{
	R_INPUT,
	R_OUTPUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
	struct s_cmd	*next;
	char			*heredoc;
	char			*unexpanded_cmd;
}					t_cmd;

typedef struct s_parser_state
{
	t_cmd	**cur;
	t_cmd	**head;
	t_shell	*shell;
	t_arena	**arena;
	t_token	*tok;
	t_list	**iterator;
}			t_parser_state;

typedef struct s_index
{
	char	*str;
	int		i;
	int		j;
}			t_index;

typedef struct s_exp
{
	char			*key;
	char			*value;
	struct s_exp	*next;
}					t_exp;

typedef struct s_state
{
	char	*res;
	size_t	len;
	size_t	i;
	int		quote;
	t_shell	*shell;
	t_arena	**arena;
}			t_state;

extern volatile sig_atomic_t	g_signal;

#endif