/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:05:14 by zchan             #+#    #+#             */
/*   Updated: 2025/03/19 14:52:17 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <signal.h>

// Signal Handler for ctrl-C
void    sigint_handler(int signo)
{
    (void)signo; // Ignore unused parameter
    g_signal_flag = 1;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void    sigint_heredoc(int signo)
{
    (void)signo;
    g_signal_flag = 1;
    rl_done = 1;
}

int rl_event_hd(void)
{
    if (g_signal_flag)
    {
        rl_replace_line("", 0);
        rl_redisplay();
        g_signal_flag = 0;
    }
    return (0);
}

// Set up signals for interactive mode
void	setup_sig_interactive(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, 0, sizeof(struct sigaction));
	ft_memset(&sa_quit, 0, sizeof(struct sigaction));
	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

// Set up signals for execution mode
void	setup_sig_exec(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

// Set up signals for heredoc mode
void	setup_sig_heredoc(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, 0, sizeof(struct sigaction));
	ft_memset(&sa_quit, 0, sizeof(struct sigaction));
	sa_int.sa_handler = sigint_heredoc;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
// Modular Function to Handle Input and Signals
// This function integrates signal handling (ctrl-C, ctrl-\) and ctrl-D detection
int process_input(char **input)
{
    // Use readline to display the prompt and get input
    *input = readline("minishell> ");

    // Check for EOF (ctrl-D)
    if (*input == NULL)
    {
        printf("exit\n");
        return (1); // Signal to exit the shell
    }

    // Add input to history if it's not empty
    if (**input != '\0')
    {
        add_history(*input);
    }

    // Check if a signal was caught
    if (g_signal_flag)
    {
        // Reset the flag after handling the signal
        g_signal_flag = 0;
        // Handle the signal (you can put custom behavior here)
        return (1);  // Return to indicate that a signal occurred
    }

    return (0); // Input successfully read
}

/*your code (i edit the signals first to try and test)*/

//Ignoring ctrl-\ (SIGQUIT)
void    setup_sigquit_ignore(void)
{
    signal(SIGQUIT, SIG_IGN); // Ignore SIGQUIT
}

// Handling ctrl-D (EOF)
// To handle ctrl-D, we check if the input reading function (readline) returns NULL.
int handle_eof(char *input)
{
    if (input == NULL)
    {
        printf("exit\n");
        return (1); // Signal to exit the shell
    }
    return (0); // Continue execution
}

// Wrapper function to configure signals
void handle_signals(void)
{
    struct sigaction sa;

    // Set up handler for SIGINT (ctrl-C)
    sa.sa_handler = sigint_handler;
    sa.sa_flags = SA_RESTART; // Restart interrupted system calls
    sigemptyset(&sa.sa_mask); // Clear the mask
    sigaction(SIGINT, &sa, NULL);

    // Ignore SIGQUIT (ctrl-\)
    setup_sigquit_ignore();
}




// int main(int argc, char **argv, char **envp)
// {
//     t_minishell *minishell = init_minishell(envp);
//     char *input;

//     // Set up signal handling
//     handle_signals();

//     // Main shell loop
//     while (1)
//     {
//         input = NULL;
//         // Process input and check for EOF (ctrl-D)
//         if (process_input(&input))
//         {
//             free(input);
//             break;
//         }
//         // Example: Process the command or simulate execution
//         if (ft_strcmp(input, "exit\n") == 0)
//         {
//             free(input);
//             break;
//         }
//         printf("You entered: %s", input); // Debugging output
//         free(input); // Clean up dynamically allocated input
//     }
//     free_minishell(minishell);
//     return 0;
// }


/* BELOW HERE ARE ALL REFERENCE MATERIAL

void handle_sigint(int signo)
{
    printf("\n"); // Move to a new line
    printf("shell> "); // Reset prompt
    fflush(stdout);
}

void setup_signals()
{
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sa.sa_flags = SA_RESTART; // Restart system calls if interrupted
    sigaction(SIGINT, &sa, NULL);

    signal(SIGQUIT, SIG_IGN); // Ignore ctrl-\
    
}

// Signal handling should be set up at the beginning of your main 
//  loop or immediately after initializing the shell’s state. This
//  ensures that the shell properly handles signals from the start.
int main(int argc, char **argv, char **envp) {
    // Initialize the minishell state
    t_minishell *minishell = init_minishell(envp);

    // Set up signal handling
    setup_signals();

    // Main shell loop
    while (1) {
        char input[1024];

        if (minishell->interactive) {
            printf("%s> ", minishell->cwd); // Display prompt
        }

        if (!fgets(input, sizeof(input), stdin)) { // Handle ctrl-D (EOF)
            if (minishell->interactive) printf("exit\n");
            break;
        }

        // Process the input (parse, execute, etc.)
        if (strcmp(input, "exit\n") == 0) break;

        // Example: Update last_exit_code (to mimic a successful command)
        minishell->last_exit_code = 0;
    }

    // Cleanup
    free_minishell(minishell);
    return 0;
}

*/

/* FROM MEDIUM

After finishing the execution, we can implement the signals
 (ctrl + C, ctrl + \, ctrl + D).
 For ctrl + C, ctrl + \, we can catch SIGINT and SIGQUIT respectively, but in 
 the case of ctrl + D we have to check the EOF or the end of string (i.e. NULL 
 character).
*/