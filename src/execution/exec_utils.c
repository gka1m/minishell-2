/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:11:15 by zchan             #+#    #+#             */
/*   Updated: 2025/03/06 13:10:07 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*You should call the free_ast() function whenever you are done processing 
a command. Typically, this would happen after calling execution_logic, since 
the AST is no longer needed at that point.*/
// for freeing AST tree after consuming or error
// void	free_ast(t_node *node)
// {
// 	if (!node)
// 		return ;
// 	if (node->type == PIPE)
// 	{
// 		free_ast(node->left);
// 		free_ast(node->right);
// 	}
// 	else if (node->type == REDIRECTION)
// 	{
// 		free_ast(node->child);
// 		free(node->filename);
// 	}
// 	else if (node->type == COMMAND)
// 	{
// 		free(node->command);
// 		for (char **arg = node->args; *arg; ++arg)
// 			free(*arg);
// 		free(node->args);
// 	}
// 	free(node);
// }

// fn for memory management (my version)
void	free_ast(t_ast *node)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->args)
	{
		while (node->args[i])
			free(node->args[i++]);
		free(node->args);
	}
	if (node->file)
		free(node->file);
	free(node);
}

// Integration Point:
// Here’s an example of how to integrate free_ast() into your main loop:

// Updated main:
// t_node *ast = parse_input(input); // Mock parsing function

// if (ast) {
//     execution_logic(ast, minishell);
//     free_ast(ast); // Free the AST after execution
// } else {
//     fprintf(stderr, "Error: Parsing failed\n");
// }

