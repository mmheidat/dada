/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmheidat <mmheidat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 03:43:08 by mbamatra          #+#    #+#             */
/*   Updated: 2024/08/29 08:44:08 by mmheidat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"



static int count_without_quotes(char *s)
{
	int count;
	int i;
	t_quotes inside_quotes;

	initialize_quote(&inside_quotes);
	i = 0;
	count = 0;
	if (s[i] == '\0')
		return (0);
	count = ft_strlen(s);
	while (s[i])
	{
		update_quotes(&inside_quotes, s[i]);
		if (s[i] == '\'' && inside_quotes.double_quote == 0)
			count--;
		else if (s[i] == '\"' && inside_quotes.single_quote == 0)
			count--;
		i++;
	}
	return (count);
}



char *ft_exclude_quotes(char *str)
{
	int index;
    int nstr_index;
    char *new_str;
    t_quotes quote_state;

	if (!str)
		return (NULL);
	index = 0;
	nstr_index = 0;
	new_str = malloc(count_without_quotes(str) + 1);
	if (!new_str)
		return (NULL);
	while (str[index])
	{
		update_quotes(&quote_state, str[index]);
		if (str[index] != '\'' && str[index] != '\"')
		{
			new_str[nstr_index] = str[index];
			nstr_index++;
		}
		index++;
	}
	new_str[nstr_index] = '\0';
	return (new_str);
}



void do_exclude_quotes(t_cmd *command)
{
	int		i;
	t_cmd	*current;
	t_files	*files;

	current = command;
	while (current)
	{
		files = current->files;

		i = -1;
		while (current && current->content && current->content[++i])
			current->content[i] = ft_exclude_quotes(current->content[i]);
		while (files && files->file_name)
		{
			files->file_name = ft_exclude_quotes(files->file_name);
			files = files->next;
		}
		if (current)
			current = current->next;
	}
}

void *new_malloc(size_t size)
{
    void *ptr;

    ptr = malloc(size);
    if (!ptr)
        write(2, "Malloc failed\n", 14);
	exit(1);
}

void ft_split_helper2(const char *s, char c, t_quotes inside_quotes)
{
	while (*s && ((*s != c) || inside_quotes.single_quote || inside_quotes.double_quote))
	{
		update_quotes(&inside_quotes, *s);
		s++;
	}
	return ;
}

// void	starting_parsing(char *str, t_token **head)
// {
// 	char *delimiters;
// 	char **tokens;
// 	int i;

// 	delimiters = " |\t\n<>";
// 	i = 0;
// 	tokens = ft_split(str, delimiters);
// 	if (!tokens)
// 		return ;
// 	while (tokens[i])
// 	{
// 			add_token(head, tokens[i]);
// 			free(tokens[i]);
// 			i++;
// 	}
// 	free(tokens);
// }

// void	free_tokens(t_token *head)
// {
// 	t_token *tmp;
// 	while (head)
// 	{
// 		tmp = head->next;
// 		free(head->str);
// 		free(head);
// 		head = tmp;
// 	}
// }

// t_token *create_token(char *str)
// {
// 	t_token *new_token;
// 	new_token = malloc(sizeof(t_token));
// 	if (!new_token)
// 		return(NULL);
// 	new_token->str = ft_strdup(str);
// 	if (!new_token->str)
// 	{
// 		free(new_token);
// 		return (NULL);
// 	}
// 	new_token->next = NULL;
// 	return (new_token);
// }

// void add_token(t_token **head, char *str)
// {
//     t_token *new_token;
//     t_token *last;

//     new_token = create_token(str);
//     if (!new_token)
//         return;

//     if (!*head)
//     {
//         *head = new_token;
//     }
//     else
//     {
//         last = find_last(*head);
//         last->next = new_token;
//     }
// }

// t_token	*find_last(t_token *lst)
// {
// 	if (!lst)
// 		return (NULL);
// 	while (lst->next)
// 		lst = lst->next;
// 	return (lst);
// }

