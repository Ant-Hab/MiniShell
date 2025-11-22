#ifndef MINISHELL_TYPES_H
# define MINISHELL_TYPES_H

/* ===========================
**        Standard types
** =========================== */
# include <stddef.h>  /* size_t */
# include <stdbool.h> /* bool */

/* ===========================
**        Arena Memory Types
** =========================== */
typedef struct s_arena
{
	void			*memory_block; /* start of memory block */
	size_t			buffer;        /* total size of memory block */
	size_t			offset;        /* current offset in block */
	struct s_arena	*next;         /* next arena chunk */
}	t_arena;

# define GROWTH_FACTOR 2

/* ===========================
**        Forward declarations
** =========================== */
/* t_list is provided by libft; forward-declare so types header doesn't
   require including libft.h (keeps dependency graph clean). */
typedef struct s_list	t_list;

/* ===========================
**        Environment / Shell
** =========================== */
typedef struct s_env
{
	char			*key;   /* environment variable key */
	char			*value; /* environment variable value */
	struct s_env	*next;  /* next env node */
}	t_env;

typedef struct s_shell
{
	t_env	*env;        /* environment variables linked list */
	char	**envp;		//environment array for execve
	int		exit_code;   /* last exit code */
	pid_t	pid;
	bool	running;     /* shell running flag */
}	t_shell;

/* ===========================
**        Token types
** =========================== */
typedef enum e_token_type
{
	T_WORD,        /* normal word */
	T_OPERATOR,    /* | < > << >> */
	T_QUOTE        /* quoted string */
}	t_token_type;

typedef struct s_token
{
	char			*str;   /* token string */
	t_token_type	type;   /* token type */
}	t_token;

/* ===========================
**        Parser structures
** =========================== */
typedef enum e_redir_type
{
	R_INPUT,
	R_OUTPUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;   /* type of redirection */
	char			*target;/* file or heredoc target */
	struct s_redir	*next;  /* next redirection */
}	t_redir;

typedef struct s_cmd
{
	char			**argv;  /* argument vector */
	t_redir			*redirs; /* redirections */
	struct s_cmd	*next;   /* next command */
}	t_cmd;

/* ===========================
**        Global Variables
** =========================== */
extern int	g_signal; /* last received signal */

#endif /* MINISHELL_TYPES_H */
