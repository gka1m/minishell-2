/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 08:17:26 by zchan             #+#    #+#             */
/*   Updated: 2024/05/16 11:49:53 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	src_len;
	size_t	dest_len;
	size_t	copy_len;

	dest_len = ft_strlen(dest);
	src_len = ft_strlen(src);
	if (size <= dest_len)
	{
		return (size + src_len);
	}
	copy_len = size - dest_len - 1;
	if (copy_len > src_len)
	{
		copy_len = src_len;
	}
	ft_memcpy(dest + dest_len, src, copy_len);
	dest[dest_len + copy_len] = '\0';
	return (dest_len + src_len);
}
