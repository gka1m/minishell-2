/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:24:32 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/30 12:25:28 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	create_heredoc_tempfile(char *out_path)
{
	char		count_str[16];
	static int	counter = 0;
	int			fd;

	ft_strcpy(out_path, "/tmp/minishell_heredoc_");
	ft_itoa_into(count_str, counter++);
	ft_strcat(out_path, count_str);
	unlink(out_path);
	fd = open(out_path, O_CREAT | O_RDWR | O_EXCL, 0600);
	if (fd == -1)
	{
		perror("minishell: heredoc tempfile");
		return (-1);
	}
	return (fd);
}

int	handle_heredoc_logic(t_heredoc *hd, t_minishell *shell, char *line)
{
	t_token	*temp_token;

	if (!line)
	{
		ft_putstr_fd("minishell: warning: ", 2);
		ft_putstr_fd("here-document delimited by end-of-file\n", 2);
		// g_signal_flag = 1;
		return (1);
	}
	if (ft_strcmp(line, hd->delimiter) == 0)
		return (free(line), 1);
	if (hd->hd_quoted == false)
	{
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
	else
	{
		temp_token = create_token(line, T_STRING);
		write(hd->node->heredoc_fd, temp_token->value,
			ft_strlen(temp_token->value));
		write(hd->node->heredoc_fd, "\n", 1);
		free_tokens(temp_token);
	}
	return (0);
}

void	process_heredoc_input(t_heredoc *hd, t_minishell *shell)
{
	char	*line;
	// t_token	*temp_token;

	setup_heredoc_signals();
	rl_event_hook = rl_heredoc;
	while (1)
	{
		line = readline("> ");
		// if (!line)
		// {
		// 	ft_putstr_fd("minishell: warning: ", 2);
		// 	ft_putstr_fd("here-document delimited by end-of-file\n", 2);
		// 	g_signal_flag = 1;
		// 	break ;
		// }
		// if (ft_strcmp(line, hd->delimiter) == 0)
		// {
		// 	free(line);
		// 	break ;
		// }
		// if (!hd->hd_quoted)
		// {
		// 	temp_token = create_token(line, T_STRING);
		// 	if (temp_token)
		// 	{
		// 		expand_variables(temp_token, shell);
		// 		write(hd->node->heredoc_fd, temp_token->value,
		// 			ft_strlen(temp_token->value));
		// 		write(hd->node->heredoc_fd, "\n", 1);
		// 		free_tokens(temp_token);
		// 	}
		// }
		if (g_signal_flag)
		{
			shell->last_exit_code = 130;
			free(line);
			break ;
		}
		if (handle_heredoc_logic(hd, shell, line) || g_signal_flag)
			break ;
		// else
		// {
		// 	write(hd->node->heredoc_fd, line, ft_strlen(line));
		// 	write(hd->node->heredoc_fd, "\n", 1);
		// }
		free(line);
	}
	rl_event_hook = NULL;
}

// void	process_heredocs(t_ast *ast, t_minishell *shell)
// {
// 	t_heredoc	hd;
// 	char		path[64];
// 	int			fd;

// 	if (!ast || g_signal_flag)
// 		return ;
// 	process_heredocs(ast->right, shell);
// 	process_heredocs(ast->left, shell);
// 	if (ast->type == AST_HEREDOC)
// 	{
// 		hd.delimiter = ast->file;
// 		hd.hd_quoted = ast->hd_quoted;
// 		hd.node = ast;
// 		fd = create_heredoc_tempfile(path);
// 		if (fd == -1)
// 			return ;
// 		hd.node->heredoc_fd = fd;
// 		process_heredoc_input(&hd, shell);
// 		close(fd);
// 		fd = open(path, O_RDONLY);
// 		if (fd == -1)
// 			perror("minishell: reopen heredoc");
// 		hd.node->heredoc_fd = fd;
// 		unlink(path);
// 	}
// }

void	process_heredocs(t_ast *ast, t_minishell *shell)
{
	t_heredoc	hd;
	char		path[64];
	int			fd;

	if (!ast || g_signal_flag)
		return ;
	process_heredocs(ast->left, shell);
	process_heredocs(ast->right, shell);
	if (g_signal_flag || ast->type != AST_HEREDOC)
		return ;
	hd.delimiter = ast->file;
	hd.hd_quoted = ast->hd_quoted;
	hd.node = ast;
	fd = create_heredoc_tempfile(path);
	if (fd == -1)
		return ;
	hd.node->heredoc_fd = fd;
	process_heredoc_input(&hd, shell);
	close(fd);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		perror("minishell: reopen heredoc");
	hd.node->heredoc_fd = fd;
	unlink(path);
}

void	close_unused_heredocs(t_ast *root, t_ast *current_node)
{
	if (!root)
		return ;
	if (root != current_node && root->heredoc_fd != -1)
	{
		close(root->heredoc_fd);
		root->heredoc_fd = -1;
	}
	close_unused_heredocs(root->left, current_node);
	close_unused_heredocs(root->right, current_node);
}

void	expand_heredoc_line(t_token *token, t_minishell *shell)
{
	char	*result;
	int		i;
	char	ch[2];

	if (!token || token->type != T_STRING || !token->value)
		return ;

	result = ft_strdup("");
	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == '$' && token->value[i + 1])
			handle_variable_expansion(token->value, &i, &result, shell);
		else
		{
			ch[0] = token->value[i];
			ch[1] = 0;
			append_char(&result, ch);
			i++;
		}
	}
	free(token->value);
	token->value = result;
}
