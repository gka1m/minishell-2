/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:36:31 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/04 15:29:03 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_ast(t_ast *node, int level)
{
	if (!node)
		return ;
	// Print indentation based on the level
	for (int i = 0; i < level; i++)
		printf(" ");
	// Print the node type
	switch (node->type)
	{
	case AST_CMD:
		printf("CMD: ");
		for (int i = 0; node->args && node->args[i]; i++)
			printf("%s ", node->args[i]);
		printf("\n");
		break ;
	case AST_PIPE:
		printf("PIPE:\n");
		break ;
	case AST_REDIR_IN:
		printf("REDIR_IN: %s\n", node->file);
		break ;
	case AST_REDIR_OUT:
		printf("REDIR_OUT: %s\n", node->file);
		break ;
	case AST_APPEND:
		printf("APPEND: %s\n", node->file);
		break ;
	case AST_HEREDOC:
		printf("HEREDOC: %s\n", node->file);
		break ;
	default:
		printf("UNKNOWN NODE TYPE\n");
	}
	// Recursively print left and right children
	print_ast(node->left, level + 1);
	print_ast(node->right, level + 1);
}

void	print_tokens(t_token *tokens)
{
	t_token	*temp;

	temp = tokens;
	while (temp)
	{
		printf("Token: %s, Type: %d\n", temp->value, temp->type);
		temp = temp->next;
	}
}

// void print_env_array(char **env_array)
// {
//     if (!env_array)
//     {
//         printf("Environment array is NULL\n");
//         return;
//     }

//     printf("\nCurrent Environment:\n");
//     for (int i = 0; env_array[i]; i++)
//     {
//         printf("%s\n", env_array[i]);
//     }
// }

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*shell;
	char		*input;
    // char        **split;
	// char		**array;
	t_token		*tokens;
	t_ast		*ast;
	
	((void)argc, (void)argv);

	// Initialize the shell
	shell = init_minishell(envp);
	// print_env_vars(envp);
	// print_env_vars(shell->env_list);
	// array = convert_env_to_array(shell->env_list);
	// print_env_array(array);
	
	// Main loop
	while (1)
	{
		// Step 1: Read input
		input = readline("minishell> ");
		if (input)
			add_history(input);
		tokens = tokenize(input);
		if (!tokens)
			return (1);
		// Step 4: Grammar checking
		if (!check_grammar(tokens))
		{
			printf("Syntax error\n");
			free_tokens(&tokens);
			free(input);
			continue; // Skip to the next iteration if grammar is invalid
		}
		print_tokens(tokens);
		// print_tokens(tokens);
		ast = parse_pipeline(&tokens, shell);
		if (!ast)
		{
			printf("Syntax error\n");
			free_tokens(&tokens);
			free(input);
			continue;
		}
		
		print_ast(ast, 0);
		// TODO: Add parsing and execution steps here
	}

	// Clean up before exiting
	rl_clear_history(); // Clear readline history
	free_tokens(&tokens);
	free(input);
	free_ast(ast);
	free_minishell(shell);
	return (0);
}