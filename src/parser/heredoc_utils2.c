/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:44:24 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/31 18:42:39 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	write_unquoted_heredoc(t_heredoc *hd, t_minishell *shell, char *line)
{
	t_token	*temp_token;

	temp_token = create_token(line, T_STRING);
	if (temp_token)
	{
		expand_heredoc_line(temp_token, shell);
		write(hd->node->heredoc_fd, temp_token->value,
			ft_strlen(temp_token->value));
		write(hd->node->heredoc_fd, "\n", 1);
		free_tokens(temp_token);
	}
}

void	write_quoted_heredoc(t_heredoc *hd, char *line)
{
	t_token	*temp_token;

	temp_token = create_token(line, T_STRING);
	write(hd->node->heredoc_fd, temp_token->value,
		ft_strlen(temp_token->value));
	write(hd->node->heredoc_fd, "\n", 1);
	free_tokens(temp_token);
}

int	handle_heredoc_logic(t_heredoc *hd, t_minishell *shell, char *line)
{
	if (!line)
	{
		ft_putstr_fd("minishell: warning: ", 2);
		ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(hd->delimiter, 2);
		ft_putstr_fd("')\n", 2);
		return (1);
	}
	if (ft_strcmp(line, hd->delimiter) == 0)
		return (free(line), 1);
	if (hd->hd_quoted == false)
		write_unquoted_heredoc(hd, shell, line);
	else
		write_quoted_heredoc(hd, line);
	return (0);
}
