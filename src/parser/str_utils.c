/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:00:08 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/20 16:00:28 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*append_char(char c, char *str)
{
	char	*new;
	size_t	len;

	if (!str)
		len = 0;
	else
		len = ft_strlen(str);
	new = malloc((len + 2) * sizeof(char));
	if (!new)
		return (NULL);
	ft_memcpy(new, str, len);
	new[len] = c;
	new[len + 1] = '\0';
	return (new);
}