/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtarvain <jtarvain@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 13:10:37 by jtarvain          #+#    #+#             */
/*   Updated: 2025/12/18 16:19:16 by jtarvain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_av_error(char *oldpwd);
static int	cd_error(char *oldpwd, int mode);
static int	update_env(char *oldpwd, t_shell *shell);
static void	add_oldpwd(char *oldpwd, t_shell *shell);

int	ft_cd(t_cmd *cmds, t_shell *shell)
{
	char	*oldpwd;
	t_env	*home;

	oldpwd = getcwd(NULL, 0);
	home = find_env_node("HOME", shell->env);
	if (!cmds->argv[1] || ft_strncmp(cmds->argv[1], "--", 3) == 0)
	{
		if (!home)
			return (cd_error(oldpwd, 1));
		if (!home->value || !home->value[0])
			return (cd_error(oldpwd, 1));
		if (chdir((find_env_node("HOME", shell->env)->value)))
			return (cd_error(oldpwd, 0));
		update_env(oldpwd, shell);
		return (0);
	}
	if (cmds->argv[2])
		return (cd_av_error(oldpwd));
	if (chdir(cmds->argv[1]))
		return (cd_error(oldpwd, 0));
	update_env(oldpwd, shell);
	return (0);
}

static int	update_env(char *oldpwd, t_shell *shell)
{
	char	*newpwd;
	t_env	*oldpwd_node;
	t_env	*pwd_node;

	newpwd = getcwd(NULL, 0);
	oldpwd_node = find_env_node("OLDPWD", shell->env);
	pwd_node = find_env_node("PWD", shell->env);
	if (oldpwd)
	{
		if (oldpwd_node)
			update_cd_node(oldpwd, oldpwd_node);
		else
			add_oldpwd(oldpwd, shell);
	}
	if (newpwd)
	{
		if (pwd_node)
			update_cd_node(newpwd, pwd_node);
		else
			free(newpwd);
	}
	return (0);
}

static int	cd_error(char *oldpwd, int mode)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if (mode == 1)
		ft_putstr_fd("HOME not set\n", 2);
	else
		perror("");
	if (oldpwd)
		free(oldpwd);
	return (1);
}

static int	cd_av_error(char *oldpwd)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd("too many arguments\n", 2);
	if (oldpwd)
		free(oldpwd);
	return (1);
}

static void	add_oldpwd(char *oldpwd, t_shell *shell)
{
	add_pwd("OLDPWD", shell, oldpwd);
	free(oldpwd);
}