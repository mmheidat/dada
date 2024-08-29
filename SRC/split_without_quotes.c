/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_without_quotes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbamatra <mbamatra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 02:34:09 by mbamatra          #+#    #+#             */
/*   Updated: 2024/08/28 05:17:58 by mbamatra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"
static size_t	ft_countword(char const *s, char c)
{
	size_t	count;
	size_t	i;
	t_quotes	inside_quotes;

	initialize_quote(&inside_quotes);
	i = 0;
	count = 0;
	if (s[i] == '\0')
		return (0);
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] && inside_quotes.single_quote == 0 && inside_quotes.double_quote == 0)
			count++;
		while (s[i] != c && s[i])
		{
			update_quotes(&inside_quotes, s[i]);
			i++;
		}
	}
	return (count);
}

char *ft_strtrim_quotes(char *str, char quote1, char quote2)
{
    size_t start = 0;
    size_t end;
    size_t len;

    if (!str)
        return NULL;

    len = ft_strlen(str);
    if (len == 0)
        return str;

    // Find the start index
    while (str[start] == quote1 || str[start] == quote2)
        start++;

    // Find the end index
    end = len - 1;
    while (end > start && (str[end] == quote1 || str[end] == quote2))
        end--;

    // Create a new string without the quotes
    return ft_substr(str, start, end - start + 1);
}

static void	free_split_memory(char **split, size_t i)
{
	while (i > 0)
		free(split[--i]);
	free(split);
}

static size_t	ft_len(char const *s, char c)
{
	t_quotes	inside_quotes;
	int i;

	i = 0;
	initialize_quote(&inside_quotes);
	
	while (s[i])
	{
		if (s[i] == '\'' && inside_quotes.single_quote == 0)
			inside_quotes.single_quote = 1;
		else if (s[i] == '\'' && inside_quotes.single_quote == 1)
			inside_quotes.single_quote = 0;
		else if (s[i] == '\"' && inside_quotes.double_quote == 0)
			inside_quotes.double_quote = 1;
		else if (s[i] == '\"' && inside_quotes.double_quote == 1)
			inside_quotes.double_quote = 0;
		if (s[i] == c && inside_quotes.single_quote == 0 && inside_quotes.double_quote == 0)
			break;
		i++;
	}
	return (i);
}

char **ft_split_without_quotes(char const *s, char c)
{
    char **split;
    size_t word_len;
    size_t i;
    t_quotes inside_quotes;
    if (!s)
        return NULL;
    initialize_quote(&inside_quotes);
    split = malloc((ft_countword(s, c) + 1) * sizeof(char *));
    if (!split)
        return NULL;
    i = 0;
    while (*s)
    {
        while (*s == c && *s)
            s++;

        if (*s)
        {
            const char *start = s;
            // Traverse until the end of the word or the end of the string
            while (*s && (*s != c || inside_quotes.single_quote || inside_quotes.double_quote))
            {
                // Update the quote states
                update_quotes(&inside_quotes, *s);
                s++;
            }

            word_len = s - start;
            char *word = ft_substr(start, 0, word_len);
            split[i++] = ft_strdup(word);
            if (!split[i - 1])
               return(free_split_memory(split, i), NULL);
            free(word);
        }
    }
    split[i] = NULL;
    return split;
}


