/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:34:55 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/21 15:54:28 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**convert_env_to_array(t_env *env_list)
{
	char	**env_array;
	t_env	*current;
	int		count;
	int		i;

	// Count environment variables
	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	// Allocate array (+1 for NULL terminator)
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	// Fill array
	i = 0;
	current = env_list;
	while (current)
	{
		env_array[i] = join_str(current->key, "=", current->value);
		if (!env_array[i])
			return (free_split(env_array), NULL);
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

char	*join_str(char const *s1, char const *s2, char const *s3)
{
	char    *result;
	size_t  len;
	size_t  pos;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1;
	result = malloc(len);
	if (!result)
		return (NULL);
	pos = 0;
	pos += ft_strlcpy(result + pos, s1, len - pos);
	pos += ft_strlcpy(result + pos, s2, len - pos);
	pos += ft_strlcpy(result + pos, s3, len - pos);
	return (result);
}

void wait_for_children(t_minishell *shell)
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, 0)) > 0) {
        if (WIFEXITED(status))
            shell->last_exit_code = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            shell->last_exit_code = 128 + WTERMSIG(status);
    }
}
