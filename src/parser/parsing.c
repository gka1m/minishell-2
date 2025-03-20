/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:44:21 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/20 14:50:30 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_ast   *parse_pipeline(t_token **tokens, t_minishell *shell)
{
    t_ast   *left;
    t_ast   *right;
    t_ast   *pipe_node;

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

