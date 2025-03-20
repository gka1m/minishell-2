/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:37:48 by zchan             #+#    #+#             */
/*   Updated: 2024/06/03 15:34:01 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	src_len;

	src_len = ft_strlen(src);
	if (size > 0)
	{
		if (src_len >= size)
		{
			ft_memcpy(dest, src, size - 1);
			dest[size - 1] = '\0';
		}
		else
		{
			ft_memcpy(dest, src, src_len + 1);
		}
	}
	return (src_len);
}
