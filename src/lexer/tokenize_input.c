/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:15:04 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/25 18:14:35 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_grammar(t_token *head);
int	check_heredoc(t_token *head);

// function to create a new token and fill out the necessary information
t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	if (!new->value)
		return (free(new), NULL);
	new->type = type;
	new->previous = NULL;
	new->next = NULL;
	return (new);
}

// function adds token to existing doubly linked list
void	add_token(t_token **head, t_token *new)
{
	t_token	*temp;

	if (!new)
		return ;
	if (!*head)
	{
		*head = new;
		new->previous = NULL;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	new->previous = temp;
}

// function to classify tokens according to the type
t_token_type	classify_token(char *input)
{
	if (ft_strncmp(input, "<<", 2) == 0)
		return (T_HEREDOC);
	if (ft_strncmp(input, "|", 1) == 0)
		return (T_PIPE);
	if (ft_strncmp(input, ">", 1) == 0)
		return (T_REDIR_OUT);
	if (ft_strncmp(input, "<", 1) == 0)
		return (T_REDIR_IN);
	if (ft_strncmp(input, ">>", 2) == 0)
		return (T_APPEND);
	if (ft_strncmp(input, "", 1) == 0)
		return (T_EOF);
	return (T_STRING);
}

t_token	*tokenize_input(char **split)
{
	t_token	*head;
	t_token	*new;
	t_token	*last;
	int		i;

	head = NULL;
	last = NULL;
	i = -1;
	while (split[++i])
	{
		new = create_token(split[i], classify_token(split[i]));
		if (!new)
			return (free_tokens(&head), NULL);
		if (should_concatenate(last, split[i]))
			concatenate_quoted_strings(last, new);
		else
		{
			add_token(&head, new);
			last = new;
		}
	}
	new = create_token("", T_EOF);
	if (!new)
		return (free_tokens(&head), NULL);
	return (add_token(&head, new), head);
}

// overall tokenize input function to tokenize the input
// t_token	*tokenize_input(char **split)
// {
// 	t_token	*head;
// 	t_token	*new;
// 	int		i;

// 	i = -1;
// 	head = NULL;
// 	new = NULL;
// 	while (split[++i])
// 	{
// 		new = create_token(split[i], classify_token(split[i]));
// 		if (!new)
// 		{
// 			free_tokens(&head);
// 			return (NULL);
// 		}
// 		add_token(&head, new);
// 	}
// 	new = create_token("", T_EOF);
// 	if (!new)
// 		return (free_tokens(&head), NULL);
// 	add_token(&head, new);
// 	return (head);
// }

// const char	*get_token_type_str(t_token_type type)
// {
// 	if (type == T_HEREDOC)
// 		return ("T_HEREDOC");
// 	if (type == T_PIPE)
// 		return ("T_PIPE");
// 	if (type == T_REDIR_OUT)
// 		return ("T_REDIR_OUT");
// 	if (type == T_REDIR_IN)
// 		return ("T_REDIR_IN");
// 	if (type == T_APPEND)
// 		return ("T_APPEND");
// 	return ("T_STRING"); // Default case
// }

// int	main(void)
// {
// 	char	*input;
// 	char	**split;
// 	t_token	*head;

// 	while (1)
// 	{
// 		// Prompt and read user input
// 		input = readline("minishell> ");
// 		if (!input)
// 		{
// 			printf("exit\n");
// 			break;
// 		}
// 		// Add non-empty input to history
// 		if (*input)
// 			add_history(input);

// 		// Split input into words (use your ft_split implementation)
// 		split = ft_split(input, ' ');
// 		free(input);
// 		if (!split)
// 		{
// 			printf("Error: Failed to split input.\n");
// 			continue;
// 		}

// 		// Tokenize the split input
// 		head = tokenize_input(split);
// 		free_split(split);
// 		if (!head)
// 		{
// 			printf("Error: Failed to tokenize input.\n");
// 			continue;
// 		}

// 		// Check grammar rules
// 		if (!check_grammar(head))
// 		{
// 			printf("Syntax error detected.\n");
// 			free_tokens(&head);
// 			continue;
// 		}

// 		// If syntax is correct, display tokens (for testing purposes)
// 		t_token *temp = head;
// 		while (temp)
// 		{
// 			printf("Token: %-10s Type: %d\n", temp->value, temp->type);
// 			temp = temp->next;
// 		}

// 		// Free token list after processing
// 		free_tokens(&head);
// 	}

// 	return (0);
// }