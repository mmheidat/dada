/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmheidat <mmheidat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 16:16:44 by mbamatra          #+#    #+#             */
/*   Updated: 2024/08/29 08:43:53 by mmheidat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void initialize_quote(t_quotes *inside_quotes)
{
	inside_quotes->single_quote = 0;
	inside_quotes->double_quote = 0;
}

void update_quotes(t_quotes *inside_quotes, char c)
{
    if (c == '\'' && inside_quotes->single_quote == 0 && inside_quotes->double_quote == 0)
        inside_quotes->single_quote = 1;
    else if (c == '\'' && inside_quotes->single_quote == 1 && inside_quotes->double_quote == 0)
        inside_quotes->single_quote = 0;
    else if (c == '\"' && inside_quotes->double_quote == 0 && inside_quotes->single_quote == 0)
        inside_quotes->double_quote = 1;
    else if (c == '\"' && inside_quotes->double_quote == 1 && inside_quotes->single_quote == 0)
        inside_quotes->double_quote = 0;
}

