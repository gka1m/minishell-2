/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:50:07 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/25 12:52:42 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	get_quoted_len(const char *s)
{
	int		len;
	char	quote;

	len = 1;
	quote = s[0];
	while (s[len] && s[len] != quote)
		len++;
	if (s[len] == quote)
		return (len + 1);
	return (len);
}

int	count_words(const char *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (is_quote(s[i]))
		{
			i += get_quoted_len(&s[i]);
			count++;
		}
		else if (s[i] && s[i] != c)
		{
			while (s[i] && s[i] != c && !is_quote(s[i]))
				i++;
			count++;
		}
	}
	return (count);
}

char	*get_next_word(const char *s, char c, int *i)
{
	char	*word;
	int		start;
	int		len;

	while (s[*i] == c)
		(*i)++;
	start = *i;
	if (is_quote(s[*i]))
	{
		len = get_quoted_len(&s[*i]);
		word = ft_substr(s, *i, len);
		*i += len;
	}
	else
	{
		while (s[*i] && s[*i] != c && !is_quote(s[*i]))
			(*i)++;
		word = ft_substr(s, start, *i - start);
	}
	return (word);
}

char	**split_preserve_quotes(const char *s, char c)
{
	char	**result;
	int		word_count;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	result = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = -1;
	while (++j < word_count)
	{
		result[j] = get_next_word(s, c, &i);
		if (!result[j])
		{
			while (j-- > 0)
				free(result[j]);
			return (free(result), NULL);
		}
	}
	result[j] = NULL;
	return (result);
}
