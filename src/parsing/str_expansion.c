/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:49:28 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/23 16:47:05 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	is_special_char(char c);
void	single_quotes(const char *input, int *i, bool *in_single_quotes);
void	double_quotes(const char *input, int *i, bool *in_double_quotes);
char	*expand_variable(const char *input, t_minishell *shell);

void	copy_variable_value(char *result, int *j, char *var_value)
{
	ft_strlcpy(&result[*j], var_value, ft_strlen(var_value) + 1);
	*j += ft_strlen(var_value);
}

void	skip_variable_name(const char *input, int *i)
{
	while (input[*i] && !is_special_char(input[*i]))
		(*i)++;
}

void	handle_variable(const char *input, int *i, char *result,
		t_minishell *shell)
{
	char	*var_value;
	int		j;

	j = 0; // Local variable to track the position in the result string
	(*i)++;
	var_value = expand_variable(&input[*i], shell);
	if (var_value)
	{
		copy_variable_value(result, &j, var_value);
		free(var_value);
	}
	skip_variable_name(input, i);
}

// Main function to handle quotes and concatenation
char	*expand_quotes(const char *input, t_minishell *shell)
{
	char	*result;
	int		i;
	int		j;
	bool	in_single_quotes;
	bool	in_double_quotes;

	i = 0;
	j = 0;
	in_single_quotes = false;
	in_double_quotes = false;
	result = malloc(ft_strlen(input) + 1);
	if (!result)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quotes)
			single_quotes(input, &i, &in_single_quotes);
		else if (input[i] == '\"' && !in_single_quotes)
			double_quotes(input, &i, &in_double_quotes);
		else if (input[i] == '$' && !in_single_quotes)
			handle_variable(input, &i, result, shell);
		else
			result[j++] = input[i++];
	}
	return (result[j] = '\0', result);
}

// int	main(void)
// {
// 	t_minishell shell;
// 	char *env[] = {"USER=john", "HOME=/home/john", "SHELL=/bin/bash", NULL};
// 	char *input;
// 	char *expanded;

// 	shell.env = env;
// 	shell.last_exit_code = 0; // Set last exit code to 0 (success)
// 	while (1)
// 	{
// 		input = readline("minishell> ");
// 		if (!input)
// 		{
// 			printf("exit\n");
// 			break ;
// 		}
// 		if (strlen(input) > 0)
// 		{
// 			add_history(input);
// 			expanded = expand_quotes(input, &shell);
// 			if (expanded)
// 			{
// 				printf("Expanded: %s\n", expanded);
// 				free(expanded);
// 			}
// 			else
// 			{
// 				printf("Error: Expansion failed\n");
// 			}
// 		}
// 		free(input);
// 	}
// 	return (0);
// }