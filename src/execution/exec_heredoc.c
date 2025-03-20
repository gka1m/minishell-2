/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 12:08:51 by theophane         #+#    #+#             */
/*   Updated: 2025/03/06 13:09:50 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Handle Here-Document (<<)
int handle_here_document(t_ast *ast)
{
    char buffer[1024];
    char temp_filename[] = "/tmp/minishell_heredoc_XXXXXX"; // Template for temporary file
    int fd;
    ssize_t bytes_read;

    // Create a temporary file
    fd = mkstemp(temp_filename);
    if (fd < 0)
    {
        perror("mkstemp");
        return -1;
    }

    // Read input from the user until the delimiter is encountered
    printf("heredoc> ");
    while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_read] = '\0'; // Null-terminate the buffer

        // Check if the input matches the delimiter
        if (strncmp(buffer, ast->file, strlen(ast->file)) == 0 &&
            buffer[strlen(ast->file)] == '\n')
        {
            break; // Stop reading if the delimiter is encountered
        }

        // Write the input to the temporary file
        if (write(fd, buffer, bytes_read) < 0)
        {
            perror("write");
            close(fd);
            unlink(temp_filename); // Clean up the temporary file
            return -1;
        }

        printf("heredoc> ");
    }

    // Finalize the heredoc (reopen the file and set up redirection)
    return finalize_here_document(fd, temp_filename);
}

// This helper reopens the temporary file and sets up redirection
int finalize_here_document(int fd, char *temp_filename)
{
    // Close the temporary file after writing
    close(fd);

    // Reopen the temporary file for reading
    fd = open(temp_filename, O_RDONLY);
    if (fd < 0)
    {
        perror("reopen temporary file");
        unlink(temp_filename); // Clean up the temporary file
        return -1;
    }

    // Redirect the file to standard input
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        unlink(temp_filename); // Clean up the temporary file
        return -1;
    }

    // Close the file descriptor and delete the temporary file
    close(fd);
    unlink(temp_filename);

    return 0; // Success
}

/*ANYTHING BELOW HERE IS REFERENCE AND RESEARCH MATERIAL

// heredoc research, adapted from pipex hence argv
void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}

void	make_heredoc(t_heredoc *input)
{
	int		tmp_fd;
	int		flag;
	char	*line;

	tmp_fd = open("heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (tmp_fd == -1)
		perror("heredoc.tmp creation error");
	flag = 1;
	line = NULL;
	while (flag == 1)
	{
		ft_putstr_fd("pipex heredoc > ", 1);
		line = get_next_line(0);
		if (line == NULL)
			break ;
		if ((!ft_strncmp(line, input->argv[2], ft_strlen(input->argv[2])))
			&& (ft_strlen(input->argv[2]) + 1 == ft_strlen(line)))
			flag = 0;
		else
			ft_putstr_fd(line, tmp_fd);
		free(line);
	}
	close(tmp_fd);
}

void	open_file(t_heredoc *ast)
{
	if (ast->heredoc)
	{
		make_heredoc(ast);
		ast->fd_in = open("heredoc", O_RDONLY);
		ast->fd_out = open(ast->argv[ast->argc - 1],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
	{
		ast->fd_in = open(ast->argv[1], O_RDONLY);
		ast->fd_out = open(ast->argv[ast->argc - 1],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	if (ast->fd_in == -1)
	{
		ft_printf("Error opening input file\n");
		ast->fd_in = open("/dev/null", O_RDONLY);
	}
}
*/