/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:18:53 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/31 16:35:57 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	is_quoted_string(const char *str)
{
	size_t	len;

	len = ft_strlen(str);
	return (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'')
			|| (str[0] == '"' && str[len - 1] == '"')));
}

char	*remove_delim_q(const char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (is_quoted_string(str))
		return (ft_substr(str, 1, len - 2));
	else
		return (ft_strdup(str));
}

char	**append_arg(char **args, char *new_arg)
{
	size_t	len;
	size_t	i;
	char	**new_args;

	len = 0;
	i = 0;
	while (args && args[len])
		len++;
	new_args = malloc(sizeof(char *) * (len + 2));
	if (!new_args)
		return (NULL);
	while (i < len)
	{
		new_args[i] = ft_strdup(args[i]);
		i++;
	}
	new_args[len] = ft_strdup(new_arg);
	new_args[len + 1] = NULL;
	if (args)
		free_split(args);
	return (new_args);
}

int	handle_token(t_token **temp, t_ast **cmd_node, t_ast **result,
		t_minishell *shell)
{
	t_ast	*redir_node;

	if ((*temp)->type == T_STRING)
	{
		(*cmd_node)->args = append_arg((*cmd_node)->args, (*temp)->value);
		*temp = (*temp)->next;
	}
	else if (is_redirection(*temp) || (*temp)->type == T_HEREDOC)
	{
		if ((*temp)->type == T_HEREDOC)
			redir_node = parse_heredoc(temp, *result, shell);
		else
			redir_node = parse_redirection(temp, *result, shell);
		if (!redir_node)
			return (0);
		*result = redir_node;
	}
	else
		*temp = (*temp)->next;
	return (1);
}
