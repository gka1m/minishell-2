/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:44:21 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/27 13:04:23 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_ast	*parse_pipeline(t_token **tokens, t_minishell *shell)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe_node;

	left = parse_command(tokens, shell);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == T_PIPE)
	{
		*tokens = (*tokens)->next;
		if ((*tokens)->type == T_EOF)
			return (NULL);
		right = parse_pipeline(tokens, shell);
		if (!right)
			return (NULL);
		pipe_node = create_ast_node(AST_PIPE, shell);
		pipe_node->left = left;
		pipe_node->right = right;
		left = pipe_node;
	}
	return (left);
}

t_ast	*parse_redir(t_token **tokens, t_minishell *shell)
{
	t_ast		*redir_node;
	t_ast_type	type;
	char		*expanded;

	if (!tokens || !*tokens)
		return (NULL);
	if ((*tokens)->type == T_REDIR_IN)
		type = AST_REDIR_IN;
	else if ((*tokens)->type == T_REDIR_OUT)
		type = AST_REDIR_OUT;
	else if ((*tokens)->type == T_APPEND)
		type = AST_APPEND;
	else
		return (NULL);
	redir_node = create_ast_node(type, shell);
	if (!redir_node)
		return (NULL);
	*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == T_STRING)
	{
		expanded = expand_redirection_target((*tokens)->value, shell);
		if (!expanded)
			return (free(redir_node), NULL);
		redir_node->file = expanded;
		*tokens = (*tokens)->next;
	}
	return (redir_node);
}

t_ast	*parse_hd(t_token **tokens, t_minishell *shell)
{
	t_ast	*hd_node;

	hd_node = create_ast_node(AST_HEREDOC, shell);
	if (!hd_node)
		return (NULL);
	*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == T_STRING)
	{
		hd_node->file = ft_strdup((*tokens)->value);
		if (!hd_node->file)
			return (free(hd_node), NULL);
		*tokens = (*tokens)->next;
	}
	else
		return (free(hd_node), NULL);
	return (hd_node);
}

void	parse_arguments(t_token **tokens, t_ast *cmd_node)
{
	char	*expanded;

	while (*tokens && (*tokens)->type == T_STRING)
	{
		expanded = expand_input((*tokens)->value, cmd_node->shell);
		if (!expanded)
		{
			perror("minishell: expansion error");
			return ;
		}
		add_argument(&cmd_node->args, expanded);
		*tokens = (*tokens)->next;
	}
}

t_ast	*parse_command(t_token **tokens, t_minishell *shell)
{
	t_ast	*cmd_node;
	t_ast	*redir_node;
	t_ast	*last_redir;

	cmd_node = create_ast_node(AST_CMD, shell);
	if (!cmd_node)
		return (NULL);
	parse_arguments(tokens, cmd_node);
	last_redir = NULL;
	while (*tokens && (*tokens)->type != T_PIPE && (*tokens)->type != T_EOF)
	{
		if ((*tokens)->type == T_REDIR_IN || (*tokens)->type == T_REDIR_OUT
			|| (*tokens)->type == T_APPEND)
			redir_node = parse_redir(tokens, shell);
		else if ((*tokens)->type == T_HEREDOC)
			redir_node = parse_hd(tokens, shell);
		if (!redir_node)
			return (NULL);
		if (!last_redir)
			cmd_node->left = redir_node;
		else
			last_redir->left = redir_node;
		last_redir = redir_node;
	}
	return (cmd_node);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_token		*tokens;
// 	t_ast		*ast;
// 	t_minishell	*shell;
// 	char		*input;
// 	char		**split;

// 	(void)argc;
// 	(void)argv;
// 	// ✅ Initialize the shell structure
// 	shell = init_minishell(envp);
// 	if (!shell)
// 	{
// 		fprintf(stderr, "Failed to initialize shell\n");
// 		return (EXIT_FAILURE);
// 	}
// 	// ✅ Main loop
// 	while (1)
// 	{
// 		input = readline("minishell> ");
// 		if (!input)
// 			break ; // Exit on EOF (Ctrl+D)
// 		// ✅ Tokenize the input
// 		split = ft_split(input, ' ');
// 		tokens = tokenize_input(split);
// 		free_split(split);
// 		if (!tokens)
// 		{
// 			printf("Error: Tokenization failed\n");
// 			free(input);
// 			continue ;
// 		}
// 		// ✅ Check grammar
// 		if (!check_grammar(tokens))
// 		{
// 			printf("Error: Invalid grammar\n");
// 			free_tokens(&tokens);
// 			free(input);
// 			continue ;
// 		}
// 		// ✅ Parse tokens into AST
// 		ast = parse_pipeline(&tokens, shell);
// 		if (!ast)
// 		{
// 			printf("Error: Parsing failed\n");
// 			free_tokens(&tokens);
// 			free(input);
// 			continue ;
// 		}
// 		// ✅ Debug: Print the AST
// 		printf("AST:\n");
// 		print_ast(ast, 0);
// 		// ✅ Free everything after each loop
// 		free_ast(ast);
// 		free_tokens(&tokens);
// 		free(input);
// 	}
// 	// ✅ Free shell environment and shell struct
// 	for (int i = 0; shell->env[i]; i++)
// 		free(shell->env[i]);
// 	free(shell->env);
// 	free(shell);
// 	return (0);
// }
