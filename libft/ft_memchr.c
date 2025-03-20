/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 17:22:45 by zchan             #+#    #+#             */
/*   Updated: 2024/06/03 15:23:27 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;
	unsigned char	cc;

	i = 0;
	ptr = (unsigned char *)s;
	cc = (unsigned char)c;
	while (i < n)
	{
		if (ptr[i] == cc)
		{
			return ((void *)&ptr[i]);
		}
		i++;
	}
	return (NULL);
}
