/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:50:16 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/29 11:15:44 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	update_pwds(t_minishell *shell, t_env **env_list, char *new_path)
{
	char	*oldpwd;
	char	cwd[PATH_MAX];

	oldpwd = ft_strdup(shell->cwd);
	if (!oldpwd)
		return (1);
	update_env_var(*env_list, "OLDPWD", oldpwd);
	free(oldpwd);
	if (!getcwd(cwd, sizeof(cwd)))
	{
		update_env_var(*env_list, "PWD", new_path);
		ft_strlcpy(shell->cwd, new_path, PATH_MAX);
	}
	else
	{
		update_env_var(*env_list, "PWD", cwd);
		ft_strlcpy(shell->cwd, cwd, PATH_MAX);
	}
	return (0);
}

int	change_directory(t_minishell *shell, t_env **env_list, char *path)
{
	char	*abs_path;
	int		ret;

	if (chdir(path) != 0)
	{
		if (errno == EACCES)
			ft_putstr_fd("minishell: cd: Permission denied\n", 2);
		else if (errno == ENOENT)
			ft_putstr_fd("minishell: cd: No such file or directory\n", 2);
		else if (errno == ENOTDIR)
			ft_putstr_fd("minishell: cd: Not a directory\n", 2);
		return (1);
	}
	abs_path = getcwd(NULL, 0);
	if (!abs_path)
		abs_path = ft_strdup(path);
	if (!abs_path)
		return (1);
	ret = update_pwds(shell, env_list, abs_path);
	free(abs_path);
	return (ret);
}

char	*expand_path(t_minishell *shell, t_env **env_list, char *path)
{
	char	*expanded;
	t_env	*home;

	(void)shell;
	if (path[0] == '~')
	{
		home = find_env_var(*env_list, "HOME");
		if (!home || !home->value)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (NULL);
		}
		expanded = ft_strjoin(home->value, path + 1);
		return (expanded);
	}
	return (ft_strdup(path));
}

int	b_cd(t_minishell *shell, t_env **env_list, char **args)
{
	int		ret;
	t_env	*env_var;

	if (args[1] && args[2])
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	if (!args[1] || ft_strcmp(args[1], "--") == 0 || ft_strcmp(args[1],
			"~") == 0)
	{
		env_var = find_env_var(*env_list, "HOME");
		if (!env_var || !env_var->value)
			return (ft_putstr_fd("cd: HOME not set\n", 2), 1);
		return (change_directory(shell, env_list, env_var->value));
	}
	if (ft_strcmp(args[1], "-") == 0)
	{
		env_var = find_env_var(*env_list, "OLDPWD");
		if (!env_var || !env_var->value)
			return (ft_putstr_fd("cd: OLDPWD not set\n", 2), 1);
		ft_putendl_fd(env_var->value, 1);
		return (change_directory(shell, env_list, env_var->value));
	}
	ret = change_directory(shell, env_list, args[1]);
	return (ret);
}
