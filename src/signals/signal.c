/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:45:04 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/24 13:47:20 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

volatile sig_atomic_t   g_signal_flag;

/* to handle ctrl-C and ctrl-\ (initialize interactive mode)*/
void	sig_interactive(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART; // Restart system calls if interrupted
	sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;
	// signal(SIGQUIT, SIG_IGN); // Still ignore Ctrl backslash
    sigaction(SIGQUIT, &sa, NULL);
}

/* handle_sigint function (interactive input handling) */
void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_flag = 1;
		write(STDERR_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
        rl_done= 1;
	}
}

// void	sig_reset(bool for_child)
// {
// 	struct sigaction	sa;

// 	sigemptyset(&sa.sa_mask);
// 	if (for_child)
// 	{
// 		// For child processes, restore default behavior
// 		sa.sa_handler = SIG_DFL;
// 		sa.sa_flags = SA_RESTART;
// 		sigaction(SIGINT, &sa, NULL);
// 		sigaction(SIGQUIT, &sa, NULL);
// 	}
// 	else
// 	{
// 		// For parent process during child execution
// 		sa.sa_handler = SIG_IGN;
// 		sa.sa_flags = 0;
// 		sigaction(SIGINT, &sa, NULL);
// 		sigaction(SIGQUIT, &sa, NULL);
// 	}
// }

// void	setup_heredoc_signals(void)
// {
// 	struct sigaction sa;
	
// 	sa.sa_handler = handle_heredoc_sigint;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = 0;
// 	sigaction(SIGINT, &sa, NULL);
// 	sigaction(SIGQUIT, &sa, NULL);
// 	rl_catch_signals = 0;
// 	// rl_event_hook = NULL;
// }

void setup_heredoc_signals(void)
{
    struct sigaction sa;
    
    // Custom SIGINT handler for heredoc
    sa.sa_handler = handle_heredoc_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // SIGQUIT should terminate heredoc input
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
    
    rl_catch_signals = 0;
}

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_signal_flag = 1;
	// write(STDERR_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
    // rl_redisplay();
	rl_done = 1;
}

/* handles signals during execution (running a command)
resets signals to default handlers */
// void	sig_reset(bool for_child)
// {
// 	struct sigaction	sig_act;

// 	sig_act.sa_handler = SIG_DFL;
// 	sigemptyset(&sig_act.sa_mask);
// 	if (for_child == true)
// 		sig_act.sa_flags = SA_RESTART;
// 	else
// 		sig_act.sa_flags = 0;
// 	sigaction(SIGINT, &sig_act, NULL);
// 	sigaction(SIGQUIT, &sig_act, NULL);
// }

void sig_reset(bool for_child)
{
    struct sigaction sig_act;

    sigemptyset(&sig_act.sa_mask);
    
    if (for_child) {
        // For child processes (commands)
        sig_act.sa_handler = SIG_DFL;  // Default behavior
        sig_act.sa_flags = SA_RESTART;
    } else {
        // For parent process during command execution
        sig_act.sa_handler = SIG_IGN;  // Ignore signals
        sig_act.sa_flags = 0;
    }
    
    sigaction(SIGINT, &sig_act, NULL);
    sigaction(SIGQUIT, &sig_act, NULL);
}

// void	sig_ignore(void)
// {
// 	signal(SIGINT, SIG_IGN);
// 	signal(SIGQUIT, SIG_IGN);
// }

// void	sig_cmd(int sig_num)
// {
// 	if (sig_num == SIGINT)
// 		g_signal_flag = 1;
// 	write(STDERR_FILENO, "\n", 1);
// 	rl_on_new_line();
// }