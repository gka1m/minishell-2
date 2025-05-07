/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:36:31 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/07 14:42:38 by kagoh            ###   ########.fr       */
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

// int main(int argc, char **argv, char **envp)
// {
//     t_minishell *shell;
//     char *input;
//     t_token *tokens;
//     t_token *current;
//     t_ast *ast;
//     int exit_status = 0;

//     (void)argc;
//     (void)argv;

//     // Initialize the shell
//     shell = init_minishell(envp);
//     if (!shell)
//     {
//         ft_putstr_fd("minishell: initialization failed\n", STDERR_FILENO);
//         return (1);
//     }
//     rl_catch_signals = 0;
//     // Main loop
//     while (1)
//     {
//         sig_interactive();
//         // Reset signal flag and get input
//         g_signal_flag = 0;
//         input = readline("minishell> ");
//         if (!input)  // Handle Ctrl+D
//         {
//             ft_putstr_fd("exit\n", STDOUT_FILENO);
//             // exit(0);
//             break ;
//         }
//         // Skip empty input
//         if (ft_strlen(input) == 0 || g_signal_flag == 1)
//         {
//             free(input);
//             continue;
//         }

//         // Add to history if not empty
//         add_history(input);

//         // Tokenization
//         tokens = tokenize(input);
//         if (!tokens)
//         {
//             free(input);
//             continue;
//         }

//         // Grammar checking
//         if (!check_grammar(tokens))
//         {
//             shell->last_exit_code = 2;  // Syntax error code
//             free_tokens(&tokens);
//             free(input);
//             continue;
//         }
//         current = expand_all_tokens(tokens, shell);
// 		print_tokens(tokens);
//         // Parsing
//         ast = parse_pipeline(&tokens, shell);
//         if (!ast)
//         {
//             shell->last_exit_code = 2;
//             free(input);
//             free_tokens(&shell->tokens);  // Tokens are no longer needed after parsing
//             continue;
//         }
// 		// print_ast(ast, 0);
//         process_heredocs(ast, shell);
//         if (g_signal_flag)
//         {
//             shell->last_exit_code = 130;
//             free_ast(shell->ast);
//             free_tokens(&shell->tokens);
//             free(input);
//             continue;
//         }
//         // Execution
//         print_ast(ast, 0);
//         free(input);
//         exit_status = execution_logic(ast, shell);
//         // Cleanup for this iteration
//         if (shell->tokens)
//             free_tokens(&shell->tokens);
//         if (shell->ast)
//             free_ast(shell->ast);
//     }
//     // Final cleanup
//     rl_clear_history();
//     // cleanup_and_exit(shell, exit_status);
//     free_minishell(shell);
//     return (exit_status);
//     // return (0);
// }

int main(int argc, char **argv, char **envp)
{
    t_minishell *shell;
    char *input;
    int exit_status = 0;

    (void)argc;
    (void)argv;

    shell = init_minishell(envp);
    if (!shell)
    {
        ft_putstr_fd("minishell: initialization failed\n", STDERR_FILENO);
        return (1);
    }
    rl_catch_signals = 0;

    while (1)
    {
        sig_interactive();
        g_signal_flag = 0;
        input = readline("minishell> ");
        if (!input)
        {
            // ft_putstr_fd("exit\n", STDOUT_FILENO);
            // if (shell->tokens)
            //     free_tokens(shell->tokens);
            break;
        }

        if (ft_strlen(input) == 0 || g_signal_flag == 1)
        {
            free(input);
            continue;
        }

        add_history(input);

        // Tokenization
        shell->tokens = tokenize(input);
		free(input);
        if (!shell->tokens)
        {
            continue;
        }

        // Grammar checking
        if (!check_grammar(shell->tokens))
        {
            shell->last_exit_code = 2;
            free_tokens(shell->tokens);
            continue;
        }

        // Expansion
        expand_all_tokens(shell->tokens, shell);
        print_tokens(shell->tokens);

        // Parsing
        shell->ast = parse_pipeline(&shell->tokens, shell);
        if (!shell->ast)
        {
            shell->last_exit_code = 2;
            continue;
        }
        free_tokens(shell->tokens);
        // Heredocs
        process_heredocs(shell->ast, shell);
        if (g_signal_flag == 1)
        {
            shell->last_exit_code = 130;
            free_ast(shell->ast);
			free_tokens(shell->tokens);
            continue;
        }
        // Execution
        print_ast(shell->ast, 0);
        
        exit_status = execution_logic(shell->ast, shell);
        // Cleanup after each command
		// free_tokens(shell->tokens);
        free_ast(shell->ast);
    }

    // Final cleanup
    rl_clear_history();
    free_minishell(shell);
    return (exit_status);
}