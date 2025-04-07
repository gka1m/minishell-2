/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:45:04 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/07 12:27:42 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

volatile sig_atomic_t   g_signal_flag;

/* to handle ctrl-C and ctrl-\ (initialize interactive mode)*/
void    sig_interactive(void)
{
    signal(SIGINT, handle_sigint); //ctrl-c
    signal(SIGQUIT, SIG_IGN); // ctrl-backslash
}

/* handle_sigint function (interactive input handling) */
void    handle_sigint(int sig_num)
{
    (void)sig_num;
    g_signal_flag = 1;
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

/* handles signals during execution (running a command)
resets signals to default handlers */
void    sig_reset(bool for_child)
{
    struct sigaction    sig_act;

    sig_act.sa_handler = SIG_DFL;
    sigemptyset(&sig_act.sa_mask);
    if (for_child == true)
        sig_act.sa_flags = SA_RESTART;
    else
        sig_act.sa_flags = 0;
    sigaction(SIGINT, &sig_act, NULL);
    sigaction(SIGQUIT, &sig_act, NULL);
}

void    sig_ignore(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void    sig_cmd(int sig_num)
{
    if (sig_num == SIGINT)
        g_signal_flag = 1;
    write(STDERR_FILENO, "\n", 1);
    rl_on_new_line();   
}

void handle_heredoc_sigint(int sig)
{
    (void)sig;
    g_signal_flag = 1;
    write(STDERR_FILENO, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
}