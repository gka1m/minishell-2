/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 11:20:34 by zchan             #+#    #+#             */
/*   Updated: 2024/06/03 19:07:06 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_to_skip_over(char c, const char *set)
{
	while (*set != '\0')
	{
		if (c == *set)
		{
			return (1);
		}
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	char	*start;
	char	*end;
	size_t	size;

	if (!s1 || !set)
		return (NULL);
	while (*s1 != '\0' && ft_to_skip_over(*s1, set))
		s1++;
	start = (char *)s1;
	end = (char *)s1 + ft_strlen(s1) - 1;
	while (end > start && ft_to_skip_over(*end, set))
		end--;
	size = end - start + 1;
	str = malloc(size + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, start, size);
	str[size] = '\0';
	return (str);
}
