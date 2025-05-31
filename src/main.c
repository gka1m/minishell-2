/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:36:31 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/31 18:43:23 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

char	*pre_token(int *exit_status, t_minishell *shell)
{
	char	*input;

	sig_interactive();
	input = readline("minishell> ");
	if (g_signal_flag == 1)
	{
		shell->last_exit_code = 130;
		*exit_status = 130;
		g_signal_flag = 0;
		if (input)
			return (input);
		return (NULL);
	}
	if (!input)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		*exit_status = shell->last_exit_code;
		return (NULL);
	}
	add_history(input);
	return (input);
}

int	lex_and_expand(t_minishell *shell, char *input)
{
	shell->tokens = tokenize(input);
	free(input);
	if (!shell->tokens)
		return (0);
	if (!check_grammar(shell->tokens))
	{
		shell->last_exit_code = 2;
		free_tokens(shell->tokens);
		return (0);
	}
	expand_all_tokens(shell->tokens, shell);
	return (1);
}

int	parse_and_exec(t_minishell *shell, int *exit_status)
{
	shell->ast = parse_pipeline(&shell->tokens, shell);
	if (!shell->ast)
	{
		shell->last_exit_code = 2;
		return (0);
	}
	process_heredocs(shell->ast, shell);
	if (g_signal_flag == 1)
	{
		shell->last_exit_code = 130;
		g_signal_flag = 0;
		free_ast(shell->ast);
		free_tokens(shell->tokens);
		return (0);
	}
	*exit_status = execution_logic(shell->ast, shell);
	free_tokens(shell->tokens);
	free_ast(shell->ast);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*shell;
	char		*input;
	int			exit_status;

	if (argc > 1)
		return (1);
	(void)argv;
	exit_status = 0;
	shell = init_minishell(envp);
	g_signal_flag = 0;
	if (!shell)
		return (ft_putstr_fd("minishell: init failed\n", 2), free(shell), 1);
	while (1)
	{
		input = pre_token(&exit_status, shell);
		if (!input)
			break ;
		if (!lex_and_expand(shell, input))
			continue ;
		if (!parse_and_exec(shell, &exit_status))
			continue ;
	}
	return (rl_clear_history(), free_minishell(shell), exit_status);
}

// void	print_ast(t_ast *node, int level)
// {
// 	if (!node)
// 		return ;
// 	// Print indentation based on the level
// 	for (int i = 0; i < level; i++)
// 		printf(" ");
// 	// Print the node type
// 	switch (node->type)
// 	{
// 	case AST_CMD:
// 		printf("CMD: ");
// 		for (int i = 0; node->args && node->args[i]; i++)
// 			printf("%s ", node->args[i]);
// 		printf("\n");
// 		break ;
// 	case AST_PIPE:
// 		printf("PIPE:\n");
// 		break ;
// 	case AST_REDIR_IN:
// 		printf("REDIR_IN: %s\n", node->file);
// 		break ;
// 	case AST_REDIR_OUT:
// 		printf("REDIR_OUT: %s\n", node->file);
// 		break ;
// 	case AST_APPEND:
// 		printf("APPEND: %s\n", node->file);
// 		break ;
// 	case AST_HEREDOC:
// 		printf("HEREDOC: %s\n", node->file);
// 		break ;
// 	default:
// 		printf("UNKNOWN NODE TYPE\n");
// 	}
// 	// Recursively print left and right children
// 	print_ast(node->left, level + 1);
// 	print_ast(node->right, level + 1);
// }

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

// int main(int argc, char **argv, char **envp)
// {
//     t_minishell *shell;
//     char *input;
//     int exit_status = 0;

//     // (void)argc;
//     (void)argv;
// 	if (argc > 1)
// 		return (1);
//     shell = init_minishell(envp);
//     if (!shell)
//     {
//         ft_putstr_fd("minishell: initialization failed\n", STDERR_FILENO);
//         return (free(shell), 1);
//     }
//     // rl_catch_signals = 0;

// 	g_signal_flag = 0;
//     while (1)
//     {
// 		sig_interactive();
//         input = readline("minishell> ");
// 		if (g_signal_flag == 1)
// 		{
// 			free(input);
// 			exit_status = 130;
// 			g_signal_flag = 0;
// 			continue;
// 		}
//         if (!input)
//         {
//             ft_putstr_fd("exit\n", STDOUT_FILENO);
//             // if (shell->tokens)
//             //     free_tokens(shell->tokens);
//             break;
//         }

//         add_history(input);

//         // Tokenization
//         shell->tokens = tokenize(input);
// 		free(input);
//         if (!shell->tokens)
//         {
// 			// free_tokens(shell->tokens);
//             continue;
//         }

//         // Grammar checking
//         if (!check_grammar(shell->tokens))
//         {
//             shell->last_exit_code = 2;
//             free_tokens(shell->tokens);
//             continue;
//         }

//         // Expansion
//         expand_all_tokens(shell->tokens, shell);
//         print_tokens(shell->tokens);

//         // Parsing
//         shell->ast = parse_pipeline(&shell->tokens, shell);
//         if (!shell->ast)
//         {
//             shell->last_exit_code = 2;
//             continue;
//         }
//         // free_tokens(shell->tokens);

//         // Heredocs
//         process_heredocs(shell->ast, shell);
//         if (g_signal_flag == 1)
//         {
//             shell->last_exit_code = 130;
//             free_ast(shell->ast);
// 			free_tokens(shell->tokens);
// 			g_signal_flag = 0;
// 			// cleanup_and_exit(shell, 130);
//             continue;
//         }
//         // Execution
//         print_ast(shell->ast, 0);
//         exit_status = execution_logic(shell->ast, shell);
//         // Cleanup after each command
// 		// close(shell->ast->heredoc_fd);
// 		free_tokens(shell->tokens);
//         free_ast(shell->ast);
//     }

//     // Final cleanup
//     rl_clear_history();
//     free_minishell(shell);
//     return (exit_status);
// }	