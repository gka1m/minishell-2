/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 16:42:02 by zchan             #+#    #+#             */
/*   Updated: 2024/05/31 16:20:12 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_allocate_words_array(unsigned int word_count)
{
	char	**words;

	words = malloc(sizeof(char *) * (word_count + 1));
	if (!words)
	{
		return (NULL);
	}
	return (words);
}

static char	*ft_get_word(char const *str, int start, size_t len)
{
	char	*word;

	word = ft_substr(str, start, len);
	return (word);
}

static void	ft_free_words_array(char **words, unsigned int count)
{
	while (count-- > 0)
		free(words[count]);
	free(words);
}

static char	**ft_fill_words(char const *str, char **words, char spc)
{
	int				i;
	size_t			len;
	unsigned int	count;

	i = -1;
	count = 0;
	len = 0;
	while (str[++i] != '\0')
	{
		if (str[i] != spc)
			len++;
		if (str[i] != spc && (str[i + 1] == spc || str[i + 1] == '\0'))
		{
			words[count] = ft_get_word(str, i - len + 1, len);
			if (!words[count])
			{
				ft_free_words_array(words, count);
				return (NULL);
			}
			count++;
			len = 0;
		}
	}
	words[count] = NULL;
	return (words);
}

char	**ft_split(char const *s, char c)
{
	char			**array;
	int				i;
	unsigned int	word_count;

	i = -1;
	word_count = 0;
	if (!s)
		return (NULL);
	while (s[++i] != '\0')
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			word_count++;
	}
	array = ft_allocate_words_array(word_count);
	if (!array)
		return (NULL);
	return (ft_fill_words(s, array, c));
}
