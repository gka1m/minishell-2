/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:21:45 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/02 10:33:21 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void    parent_process(int pid, t_minishell *shell)
{
    int term_sig;

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    waitpid(pid, shell->last_exit_code, 0);
    if (WIFEXITED(shell->last_exit_code))
        shell->last_exit_code = WEXITSTATUS(shell->last_exit_code);
    else if (WIFSIGNALED(shell->last_exit_code))
    {
        term_sig = WTERMSIG(shell->last_exit_code);
        shell->last_exit_code = 128 + term_sig;
        if (term_sig == SIGQUIT)
            ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
    }
    setup_sig_interactive();
}

