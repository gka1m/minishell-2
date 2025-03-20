/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:36:31 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/19 14:47:25 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// void	print_tokens(t_token *tokens)
// {
// 	t_token	*temp;

// 	temp = tokens;
// 	while (temp)
// 	{
// 		printf("Token: %s, Type: %d\n", temp->value, temp->type);
// 		temp = temp->next;
// 	}
// }

int	main(int argc, char **argv, char **envp)
{
	char		*input;
    char        **split;
	t_token		*tokens;
	t_minishell	shell;

	(void)argc; // Silence unused parameter warnings
	(void)argv;

	// Initialize the shell
	shell.env = envp; // Set environment variables
	shell.last_exit_code = 0; // Initialize exit code

	// Main loop
	while (1)
	{
		// Step 1: Read input
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n"); // Handle EOF (Ctrl+D)
			break;
		}

		// Step 2: Lexical analysis (tokenization)
        split = ft_split(input, ' ');
        if (!split)
            return 1;
		tokens = tokenize_input(split);
		if (!tokens)
		{
			free_split(split);
			continue; // Skip to the next iteration if tokenization fails
		}

		// Step 3: Print tokens (for debugging)
		// print_tokens(tokens);

		// Step 4: Grammar checking
		if (!check_grammar(tokens))
		{
			printf("Syntax error\n");
			free_tokens(&tokens);
			free(input);
			continue; // Skip to the next iteration if grammar is invalid
		}

		// Step 5: Free tokens and input (for now, since we're only testing the lexer)
		free_tokens(&tokens);
		free(input);

		// TODO: Add parsing and execution steps here
	}

	// Clean up before exiting
	rl_clear_history(); // Clear readline history
	return (0);
}