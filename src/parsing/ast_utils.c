/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:18:03 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/25 17:24:30 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_ast	*create_ast_node(t_ast_type type, t_minishell *shell)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->file = NULL;
	node->left = NULL;
	node->right = NULL;
	node->shell = shell;
	return (node);
}

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
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	if (node->file)
		free(node->file);
	free(node);
}

void	add_argument(char ***args, char *value)
{
	char	**new_args;
	int		count;
	int		i;

	count = 0;
	if (*args)
		while ((*args)[count])
			count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
	i = 0;
	while (i < count)
	{
		new_args[i] = (*args)[i];
		i++;
	}
	new_args[i] = value;
	new_args[i + 1] = NULL;
	free(*args);
	*args = new_args;
}
