/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:21:45 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/03 14:28:37 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_builtin(t_minishell *shell, char **args, int fd_out)
{
	if (!args[0])
		return (0);
	if (ft_strcmp(args[0], "cd") == 0)
		return (b_cd(shell, &shell->env_list, args));
	if (ft_strcmp(args[0], "echo") == 0)
		return (b_echo(args));
	if (ft_strcmp(args[0], "env") == 0)
		return (b_env(shell->env_list));
	if (ft_strcmp(args[0], "exit") == 0)
		return (b_exit(shell, args));
	if (ft_strcmp(args[0], "export") == 0)
		return (bi_export(shell, args, fd_out));
	if (ft_strcmp(args[0], "unset") == 0)
		return (b_unset(shell->env_list, args));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (b_pwd());
	return (0);
}
