/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbamatra <mbamatra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:25:46 by mbamatra          #+#    #+#             */
/*   Updated: 2024/08/17 14:14:37 by mbamatra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

int	check_quotes(char *str)
{
	int i;
	t_quotes inside_quotes;

	i = 0;
	initialize_quote(&inside_quotes);
	while (str[i])
	{
		update_quotes(&inside_quotes, str[i]);
		i++;
	}
	if (inside_quotes.single_quote == 1 || inside_quotes.double_quote == 1)
		return (1);
	return (0);
}

int	check_redirection(char *str)
{
	int i = 0;
	t_quotes inside_quotes;
	initialize_quote(&inside_quotes);
	while (str[i])
	{
		update_quotes(&inside_quotes, str[i]);
		if ((str[i] == '>' || str[i] == '<') && inside_quotes.single_quote == 0 && inside_quotes.double_quote == 0)
		{
			if (validate_direction(str, i) == 1)
				return (1);
		}
		i++;
	}
	return (0);
}

int	validate_direction(char *str, int i)
{
	t_quotes inside_quotes;

	initialize_quote(&inside_quotes);
	update_quotes(&inside_quotes, str[i]);
	if (str[++i] == '\0')
	{
		printf("Error: redirection not valid\n");
		return (1);
	}
	if (str[i] == '>' && str[i - 1] == '>')
	{
		if (str[i] == '>')
			i++;
	}
	else if (str[i] == '<' && str[i - 1] == '<')
	{
		if (str[i] == '<')
			i++;
	}
	while (str[i] == ' ')
		i++;
	if (str[i] == '\0' || str[i] == '>' || str[i] == '<' || str[i] == '|' || str[i] == ';')
	{
		printf("Error: redirection not valid\n");
		return (1);
	}
	return (0);
}

int	check_pipes(char *str)
{
	int i;
	int pipe;
	t_quotes inside_quotes;

	i = 0;
	initialize_quote(&inside_quotes);
	pipe = 0;
	while (*str == ' ' && *str)
		str++;
	while (str[i])
	{
		update_quotes(&inside_quotes, str[i]);
		if (str[i] == '|' && !inside_quotes.single_quote && !inside_quotes.double_quote)
		{
			if (i == 0)
				return (1);
			if (str[i + 1] == '|')
				return (1);
			i++;
			while (str[i] == ' ')
				i++;
			if (str[i] == '\0' || str[i] == '|')
				return (1);
		}
		else
			i++;
	}
	return (0);
}

int	parsing(char *str)
{
	if (check_quotes(str) == 1)
		return (1);
	if (check_redirection(str) == 1)
		return (1);
	if (check_pipes(str) == 1)
		return (1);
	return (0);
}