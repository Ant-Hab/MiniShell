/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:05:51 by musajid           #+#    #+#             */
/*   Updated: 2025/12/18 16:05:51 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_to_end(t_shell *shell, t_env *node);
static char	*determine_pwd(char *cwd, char *oldpwd);

static int	init_exec_struct(t_shell *shell)
{
	shell->exec = ft_calloc(1, sizeof(t_exec));
	if (!shell->exec)
		return (1);
	shell->exec->fd = ft_calloc(1, sizeof(t_fd));
	if (!shell->exec->fd)
	{
		free(shell->exec);
		return (1);
	}
	init_fd(shell->exec->fd);
	return (0);
}

int	init_shell_and_arena(t_shell *shell, t_arena **arena, char **envp)
{
	shell->env = NULL;
	shell->exit_code = 0;
	shell->running = true;
	shell->exit_flow = FLOW_OK;
	shell->should_print_exit_message = false;
	*arena = init_arena(1024);
	if (!*arena)
	{
		ft_printf("minishell: failed to initialize memory arena\n");
		return (1);
	}
	shell->env = init_env(envp);
	if (!shell->env)
		if (!add_pwd("PWD", shell, NULL))
			return (free_arena(arena), 1);
	if (init_exec_struct(shell) != 0)
	{
		free_env(shell->env);
		free_arena(arena);
		return (1);
	}
	return (0);
}

int	add_pwd(const char *str, t_shell *shell, char *oldpwd)
{
	char	*pwd_value;
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (0);
	pwd_value = getcwd(NULL, 0);
	if (!pwd_value)
		return (free(new), 0);
	new->key = ft_strdup(str);
	if (!new->key)
	{
		free(pwd_value);
		return (free(new), 0);
	}
	new->value = determine_pwd(pwd_value, oldpwd);
	new->next = NULL;
	if (!shell->env)
		shell->env = new;
	else
		add_to_end(shell, new);
	return (1);
}

static void	add_to_end(t_shell *shell, t_env *node)
{
	t_env	*last;

	last = shell->env;
	while (last->next)
		last = last->next;
	last->next = node;
}

static char	*determine_pwd(char *cwd, char *oldpwd)
{
	char	*new;

	if (!oldpwd)
		return (cwd);
	new = ft_strdup(oldpwd);
	return (free(cwd), new);
}
