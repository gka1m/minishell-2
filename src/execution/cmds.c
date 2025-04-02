/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:21:45 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/02 15:58:22 by kagoh            ###   ########.fr       */
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

void    parent_process(int pid, t_minishell *shell)
{
    int terminator;
    int status;

    sig_ignore();
    status = shell->last_exit_code;
    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        status = 1;
        return ;
    }
    if (WIFEXITED(status))
        status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
    {
        terminator = WTERMSIG(status);
        shell->last_exit_code = 128 + terminator;
        if (terminator == SIGQUIT)
            ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
        else if (terminator == SIGINT)
            ft_putstr_fd("\n", STDERR_FILENO);
    }
    g_signal_flag = 0;
    sig_interactive();
}

