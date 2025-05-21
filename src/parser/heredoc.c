/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:24:32 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/21 11:48:55 by kagoh            ###   ########.fr       */
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
// 		if (strcmp(line, hd->delimiter) == 0)
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
// 		return;
//     if (ast->type == AST_HEREDOC)
//     {
//         hd.delimiter = ast->file;
//         hd.hd_quoted = ast->hd_quoted;
//         if (!create_heredoc_pipe(&hd))
//             return;
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

void	ft_itoa_into(char *buf, int n)
{
	int		len = 0;
	int		num = n;
	char	tmp[16];

	if (n == 0)
	{
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}

	if (n < 0) // You likely won't need this, since getpid and counter are >= 0
	{
		buf[len++] = '-';
		n = -n;
	}

	int i = 0;
	while (num > 0)
	{
		tmp[i++] = '0' + (num % 10);
		num /= 10;
	}

	// Reverse tmp into buf
	for (int j = 0; j < i; j++)
		buf[len++] = tmp[i - j - 1];
	buf[len] = '\0';
}

void	ft_strcpy(char *dst, const char *src)
{
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
}

void	ft_strcat(char *dst, const char *src)
{
	while (*dst)
		dst++;
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
}

int	create_heredoc_tempfile(t_minishell *shell)
{
	char	temp_path[64];
	char	pid_str[16];
	char	count_str[16];
	static int	counter = 0;
	int		fd;

	(void)shell;
	// Convert getpid() to string
	ft_itoa_into(pid_str, getpid());

	// Convert counter to string
	ft_itoa_into(count_str, counter++);

	// Build path manually: "/tmp/minishell_heredoc_" + pid + "_" + counter
	ft_strcpy(temp_path, "/tmp/minishell_heredoc_");
	ft_strcat(temp_path, pid_str);
	ft_strcat(temp_path, "_");
	ft_strcat(temp_path, count_str);

	// Create the file
	fd = open(temp_path, O_CREAT | O_RDWR | O_EXCL, 0600);
	if (fd == -1)
	{
		perror("minishell: heredoc tempfile");
		return -1;
	}
	unlink(temp_path); // Remove dir entry but keep fd valid
	return fd;
}

void process_heredoc_input(t_heredoc *hd, t_minishell *shell)
{
    char *line;
    t_token *temp_token;
    // int fd = create_heredoc_tempfile(shell);

    // if (fd == -1)
    //     cleanup_and_exit(shell, 1);
	// else
	// 	hd->node->heredoc_fd = fd; // Store FD in AST
	// hd->node->heredoc_fd = create_heredoc_tempfile(shell);
    setup_heredoc_signals();
    while (!g_signal_flag)
    {
        line = readline("> ");
        // if (g_signal_flag)
        // {
        //     free(line);
        //     close(fd);
		// 	// g_signal_flag = 0;
        //     // cleanup_and_exit(shell, 130);
		// 	// shell->last_exit_code = 130;
		// 	break ;
        // }
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n",
							STDERR_FILENO);
			g_signal_flag = 1;
			// cleanup_and_exit(shell, shell->last_exit_code);
			break;	
		}

        if (strcmp(line, hd->delimiter) == 0)
        {
            free(line);
            break;
        }

        if (!hd->hd_quoted)
        {
            temp_token = create_token(line, T_STRING);
            if (temp_token)
            {
                expand_variables(temp_token, shell);
                write(hd->node->heredoc_fd, temp_token->value, ft_strlen(temp_token->value));
                write(hd->node->heredoc_fd, "\n", 1);
                free_tokens(temp_token);
            }
        }
        else
        {
            write(hd->node->heredoc_fd, line, strlen(line));
            write(hd->node->heredoc_fd, "\n", 1);
        }
        free(line);
    }
    lseek(hd->node->heredoc_fd, 0, SEEK_SET); // Rewind for reading
	// close(fd);
}

void  process_heredocs(t_ast *ast, t_minishell *shell)
{
    t_heredoc hd;

    if (!ast)
        return;

    if (ast->type == AST_HEREDOC)
    {
        hd.delimiter = ast->file;
        hd.hd_quoted = ast->hd_quoted;
        hd.node = ast;
        hd.node->heredoc_fd = create_heredoc_tempfile(shell);
        // Process directly in parent (no fork)
        process_heredoc_input(&hd, shell);
    }
	// close(hd.node->heredoc_fd);
    // else
    // {
    //     process_heredocs(ast->left, shell);
    //     process_heredocs(ast->right, shell);
	// }
	process_heredocs(ast->left, shell);
	process_heredocs(ast->right, shell);
}

void close_unused_heredocs(t_ast *root, t_ast *current_node)
{
    if (!root) return;

    // Close heredocs that aren't needed for current command
    if (root != current_node && root->heredoc_fd != -1) {
        close(root->heredoc_fd);
        root->heredoc_fd = -1;
    }

    // Recursively check left and right subtrees
    close_unused_heredocs(root->left, current_node);
    close_unused_heredocs(root->right, current_node);
}