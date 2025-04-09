/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 16:38:29 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/09 11:24:59 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	remove_quotes(char *str)
{
	char	*src;
	char	*dst;
	char	quote;

	src = str;
	dst = str;
	quote = 0;
	if (!str || !*str)
		return ;
	while (*src)
	{
		if (!quote && (*src == '\'' || *src == '\"'))
			quote = *src;
		else if (quote && *src == quote)
			quote = 0;
		else
			*dst++ = *src;
		src++;
	}
	*dst = '\0';
}

