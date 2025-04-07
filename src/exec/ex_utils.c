/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:55:22 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/07 15:59:23 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_builtin(t_minishell *shell, char **args, int fd_out)
{
	if (!args[0])
		return (0);
	if (ft_strncmp(args[0], "cd", 2) == 0)
		return (b_cd(shell, &shell->env_list, args));
	if (ft_strncmp(args[0], "echo", 4) == 0)
		return (b_echo(args));
	if (ft_strncmp(args[0], "env", 3) == 0)
		return (b_env(shell->env_list));
	if (ft_strncmp(args[0], "exit", 4) == 0)
		return (b_exit(shell, args));
	if (ft_strncmp(args[0], "export", 6) == 0)
		return (bi_export(shell, args, fd_out));
	if (ft_strncmp(args[0], "unset", 5) == 0)
		return (b_unset(shell->env_list, args));
	if (ft_strncmp(args[0], "pwd", 3) == 0)
		return (b_pwd());
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strncmp(cmd, "cd", 2) == 0 || ft_strncmp(cmd, "echo", 4) == 0
		|| ft_strncmp(cmd, "env", 3) == 0 || ft_strncmp(cmd, "exit", 4) == 0
		|| ft_strncmp(cmd, "pwd", 3) == 0 || ft_strncmp(cmd, "export", 6) == 0
		|| ft_strncmp(cmd, "unset", 5) == 0);
}

