/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:21:45 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/02 14:24:07 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

