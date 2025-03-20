/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:54:19 by zchan             #+#    #+#             */
/*   Updated: 2024/05/30 17:09:09 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total_memory;
	void	*ptr;

	total_memory = nmemb * size;
	if (nmemb != 0 && size != 0 && total_memory / nmemb != size)
	{
		return (NULL);
	}
	ptr = malloc(total_memory);
	if (!ptr)
	{
		return (NULL);
	}
	ft_memset(ptr, 0, total_memory);
	return (ptr);
}

/*
void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*return_ptr;

	if (size && nmemb >= 2147483647 / size)
		return (NULL);
	return_ptr = malloc(nmemb * size);
	if (!return_ptr)
		return (NULL);
	ft_bzero(return_ptr, nmemb * size);
	return (return_ptr);
}
*/
