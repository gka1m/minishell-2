/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:24:32 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/29 09:36:13 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// int	create_heredoc_pipe(t_heredoc *hd)
// {
// 	if (pipe(hd->pipefd) == -1)
// 	{
// 		perror("minishell: pipe");
// 		return (0);
// 	}
// 	return (1);
// }

// void	process_heredoc_input(t_heredoc *hd, t_minishell *shell)
// {
// 	char	*line;
// 	t_token	*temp_token;

// 	setup_heredoc_signals();
// 	while (!g_signal_flag)
// 	{
// 		line = readline("> ");
// 		if (g_signal_flag)
// 		{
// 			free(line);
// 			close(hd->pipefd[1]);
// 			close(hd->pipefd[0]);
// 			cleanup_and_exit(shell, 130);
// 		}
// 		if (!line)
// 		{
// 			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n",
// 				STDERR_FILENO);
// 			g_signal_flag = 1;
// 			close(hd->pipefd[0]);
// 			close(hd->pipefd[1]);
// 			cleanup_and_exit(shell, shell->last_exit_code);
// 			// break ;
// 		}
// 		if (ft_strcmp(line, hd->delimiter) == 0)
// 		{
// 			free(line);
// 			close(hd->pipefd[0]);
// 			close(hd->pipefd[1]);
// 			break ;
// 		}
// 		// Write to pipe only
// 		if (!hd->hd_quoted)
// 		{
// 			temp_token = create_token(line, T_STRING);
// 			if (temp_token)
// 			{
// 				expand_variables(temp_token, shell);
// 				write(hd->pipefd[1], temp_token->value,
// 					ft_strlen(temp_token->value));
// 				write(hd->pipefd[1], "\n", 1);
// 				free_tokens(temp_token);
// 			}
// 		}
// 		else
// 		{
// 			write(hd->pipefd[1], line, ft_strlen(line));
// 			write(hd->pipefd[1], "\n", 1);
// 		}
// 		free(line);
// 	}
// 	close(hd->pipefd[0]);
// 	// close(hd->pipefd[1]);
// 	// if (g_signal_flag)
// 	// 	exit(130);
// 	// exit(0);
// 	// rl_replace_line("", 0);
// 	// rl_redisplay();
// }

// void process_heredocs(t_ast *ast, t_minishell *shell)
// {
//     t_heredoc hd;
//     pid_t pid;
//     int status;

//     if (!ast)
// 		return ;
//     if (ast->type == AST_HEREDOC)
//     {
//         hd.delimiter = ast->file;
//         hd.hd_quoted = ast->hd_quoted;
//         if (!create_heredoc_pipe(&hd))
//             return ;
//         pid = fork();
//         if (pid == 0) {
//             // Child process - collect heredoc content
//             setup_heredoc_signals();
// 			close(hd.pipefd[0]);
//             process_heredoc_input(&hd, shell);
// 			// free_minishell(shell);
//             // exit(0);
// 			close(hd.pipefd[1]);
// 			cleanup_and_exit(shell, 0);
//         } else {
//             // Parent process
//             close(hd.pipefd[1]); // Close write end
//             waitpid(pid, &status, 0);

//             if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
//                 ast->heredoc_fd = hd.pipefd[0]; // Store read end
//             } else {
//                 close(hd.pipefd[0]);
//                 if (WIFSIGNALED(status)) {
// 					shell->last_exit_code = 128 + WTERMSIG(status);
//                 }
// 				close(ast->heredoc_fd);
// 				return ;
//             }
//         }
//     }
// 	// close(hd.pipefd[0]);
//     process_heredocs(ast->left, shell);
//     process_heredocs(ast->right, shell);
// }

// int	create_heredoc_tempfile(t_minishell *shell)
// {
// 	char		temp_path[64];
// 	char		pid_str[16];
// 	char		count_str[16];
// 	static int	counter;
// 	int			fd;

// 	(void)shell;
// 	counter = 0;
// 	ft_itoa_into(pid_str, getpid());
// 	ft_itoa_into(count_str, counter++);
// 	ft_strcpy(temp_path, "/tmp/minishell_heredoc_");
// 	ft_strcat(temp_path, pid_str);
// 	ft_strcat(temp_path, "_");
// 	ft_strcat(temp_path, count_str);
// 	fd = open(temp_path, O_CREAT | O_RDWR | O_EXCL, 0600);
// 	if (fd == -1)
// 	{
// 		perror("minishell: heredoc tempfile");
// 		return (-1);
// 	}
// 	unlink(temp_path);
// 	return (fd);
// }

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
			expand_variables(temp_token, shell);
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
	while (!g_signal_flag)
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
		if (handle_heredoc_logic(hd, shell, line))
			break ;
		// else
		// {
		// 	write(hd->node->heredoc_fd, line, ft_strlen(line));
		// 	write(hd->node->heredoc_fd, "\n", 1);
		// }
		free(line);
	}
}

// void	process_heredoc_input(t_heredoc *hd, t_minishell *shell)
// {
// 	char	*line;
// 	t_token	*temp_token;

// 	setup_heredoc_signals();
// 	while (!g_signal_flag)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 		{
// 			ft_putstr_fd("minishell: warning: ", 2);
// 			ft_putstr_fd("here-document delimited by end-of-file\n", 2);
// 			g_signal_flag = 1;
// 			break ;
// 		}
// 		if (ft_strcmp(line, hd->delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		if (!hd->hd_quoted)
// 		{
// 			temp_token = create_token(line, T_STRING);
// 			if (temp_token)
// 			{
// 				expand_variables(temp_token, shell);
// 				write(hd->node->heredoc_fd, temp_token->value,
// 					ft_strlen(temp_token->value));
// 				write(hd->node->heredoc_fd, "\n", 1);
// 				free_tokens(temp_token);
// 			}
// 		}
// 		else
// 		{
// 			write(hd->node->heredoc_fd, line, ft_strlen(line));
// 			write(hd->node->heredoc_fd, "\n", 1);
// 		}
// 		free(line);
// 	}
// 	lseek(hd->node->heredoc_fd, 0, SEEK_SET);
// }

// void	process_heredoc_input(t_heredoc *hd, t_minishell *shell)
// {
// 	char	*line;
// 	t_token	*temp_token;
// 	int		write_fd;
// 	int		read_fd;

// 	setup_heredoc_signals();
// 	write_fd = hd->node->heredoc_fd;
// 	read_fd = dup(write_fd);
// 	if (read_fd == -1)
// 	{
// 		perror("dup");
// 		cleanup_and_exit(shell, 1);
// 	}
// 	while (!g_signal_flag)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 		{
// 			ft_putstr_fd("minishell: warning: ", 2);
// 			ft_putstr_fd("here-document delimited by end-of-file\n", 2);
// 			g_signal_flag = 1;
// 			break ;
// 		}
// 		if (ft_strcmp(line, hd->delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		if (!hd->hd_quoted)
// 		{
// 			temp_token = create_token(line, T_STRING);
// 			if (temp_token)
// 			{
// 				expand_variables(temp_token, shell);
// 				write(write_fd, temp_token->value,
					// ft_strlen(temp_token->value));
// 				write(write_fd, "\n", 1);
// 				free_tokens(temp_token);
// 			}
// 		}
// 		else
// 		{
// 			write(write_fd, line, strlen(line));
// 			write(write_fd, "\n", 1);
// 		}
// 		free(line);
// 	}
// 	close(write_fd);
// 	hd->node->heredoc_fd = read_fd;
// }

// void process_heredoc_input(t_heredoc *hd, t_minishell *shell)
// {
//     char *line;
//     t_token *temp_token;
//     int pipefd[2];

//     if (pipe(pipefd) == -1) {
//         perror("minishell: pipe");
//         cleanup_and_exit(shell, 1);
//     }

//     setup_heredoc_signals();
//     while (!g_signal_flag)
//     {
//         line = readline("> ");
//         if (!line)
//         {
//             ft_putstr_fd("minishell: warning: ", 2);
//             ft_putstr_fd("here-document delimited by end-of-file\n", 2);
//             g_signal_flag = 1;
//             break ;
//         }

//         if (ft_strcmp(line, hd->delimiter) == 0)
//         {
//             free(line);
//             break ;
//         }

//         if (!hd->hd_quoted)
//         {
//             temp_token = create_token(line, T_STRING);
//             if (temp_token)
//             {
//                 expand_variables(temp_token, shell);
//                 write(pipefd[1], temp_token->value,
	// ft_strlen(temp_token->value));
//                 write(pipefd[1], "\n", 1);
//                 free_tokens(temp_token);
//             }
//         }
//         else
//         {
//             write(pipefd[1], line, strlen(line));
//             write(pipefd[1], "\n", 1);
//         }
//         free(line);
//     }
//     close(pipefd[1]);  /* Close write end */
//     hd->node->heredoc_fd = pipefd[0];  /* Store read end for command */
// }

// void	process_heredocs(t_ast *ast, t_minishell *shell)
// {
// 	t_heredoc	hd;

// 	if (!ast)
// 		return ;
// 	if (ast->type == AST_HEREDOC)
// 	{
// 		hd.delimiter = ast->file;
// 		hd.hd_quoted = ast->hd_quoted;
// 		hd.node = ast;
// 		hd.node->heredoc_fd = create_heredoc_tempfile(shell);
// 		process_heredoc_input(&hd, shell);
// 	}
// 	process_heredocs(ast->left, shell);
// 	process_heredocs(ast->right, shell);
// }

void	process_heredocs(t_ast *ast, t_minishell *shell)
{
	t_heredoc	hd;
	char		path[64];
	int			fd;

	if (!ast)
		return ;
	process_heredocs(ast->right, shell);
	process_heredocs(ast->left, shell);
	if (ast->type == AST_HEREDOC)
	{
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
