/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:32:28 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/15 15:35:38 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_tokens(t_token **head)
{
	t_token	*temp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		temp = *head;
		*head = (*head)->next;
		free(temp->value);
		free(temp);
	}
	*head = NULL;
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

int	check_grammar(t_token *head)
{
	if (!check_redirects(head))
		return (0);
	if (!check_append(head))
		return (0);
	if (!check_heredoc(head))
		return (0);
	if (!check_pipe(head))
		return (0);
	if (!check_eof(head))
		return (0);
	return (1);
}