/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_history.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:42:02 by gkaim             #+#    #+#             */
/*   Updated: 2025/03/25 14:08:11 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

#define BUFFER_SIZE 1024

// Load history from file
/* void load_history(const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return;

    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1)
    {
        line[strcspn(line, "\n")] = 0; // Remove newline
        add_to_history(line);
    }
    free(line);
    fclose(file);
} */

char	*extract_line(char *buffer, int *start, int end)
{
	char	*line;
	int		len;

	len = end - *start;
	line = (char *)malloc(len + 1);
	if (!line)
		return (NULL);
	strncpy(line, &buffer[*start], len);
	line[len] = '\0';
	*start = end + 1;
	return (line);
}

void	process_buffer(char *buffer, ssize_t bytes_read)
{
	int		start;
	int		i;
	char	*line;

	start = 0;
	i = 0;
	while (i < bytes_read)
	{
		if (buffer[i] == '\n' || buffer[i] == '\0')
		{
			buffer[i] = '\0';
			line = extract_line(buffer, &start, i);
			if (line)
			{
				add_to_history(line);
				free(line);
			}
		}
		i++;
	}
}

void	load_history(const char *filename)
{
	int		fd;
	ssize_t	bytes_read;
	char	buffer[BUFFER_SIZE];

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return ;
	while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
		process_buffer(buffer, bytes_read);
	close(fd);
}
