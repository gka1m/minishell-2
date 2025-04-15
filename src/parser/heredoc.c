/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:24:32 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/15 12:16:25 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	create_heredoc_pipe(t_heredoc *hd)
{
	if (pipe(hd->pipefd) == -1)
	{
		perror("minishell: pipe");
		return (0);
	}
	return (1);
}

void	process_heredoc_input(t_heredoc *hd, t_minishell *shell)
{
	char	*line;
	t_token	*temp_token;

	setup_heredoc_signals();
	while (!g_signal_flag)
	{
		line = readline("hd> ");
		if (!line || ft_strncmp(line, hd->delimiter,
				ft_strlen(line)) == 0)
		{
			free(line);
			break ;
		}
		if (!hd->hd_quoted)
		{
			// Create a temporary token for expansion
			temp_token = create_token(line, T_STRING);
			if (temp_token)
			{
				expand_variables(temp_token, shell);
				write(hd->pipefd[1], temp_token->value,
					ft_strlen(temp_token->value));
				free_tokens(&temp_token);
			}
		}
		else
			write(hd->pipefd[1], line, ft_strlen(line));
		write(hd->pipefd[1], "\n", 1);
		free(line);
	}
	close(hd->pipefd[1]);
}

void	process_heredocs(t_ast *ast, t_minishell *shell)
{
	t_heredoc	hd;

	if (!ast)
		return ;
	if (ast->type == AST_HEREDOC)
	{
		hd.delimiter = ast->file;
		hd.hd_quoted = ast->hd_quoted;
		if (!create_heredoc_pipe(&hd))
			return ;
		process_heredoc_input(&hd, shell);
		ast->heredoc_fd = hd.pipefd[0];
		close(hd.pipefd[1]);
		if (g_signal_flag)
			shell->last_exit_code = 130;
	}
	process_heredocs(ast->left, shell);
	process_heredocs(ast->right, shell);
}
