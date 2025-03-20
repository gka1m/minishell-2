/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 11:01:03 by zchan             #+#    #+#             */
/*   Updated: 2024/06/03 15:33:15 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	total_len;
	size_t	len_1;
	size_t	len_2;

	len_1 = ft_strlen(s1);
	len_2 = ft_strlen(s2);
	total_len = len_1 + len_2;
	str = malloc(total_len + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, len_1);
	ft_memcpy(str + len_1, s2, len_2);
	str[total_len] = '\0';
	return (str);
}
