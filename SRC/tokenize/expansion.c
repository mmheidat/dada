/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbamatra <mbamatra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:16:22 by mbamatra          #+#    #+#             */
/*   Updated: 2024/08/28 10:08:20 by mbamatra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

char *ft_strjoinchar(char *str, char c)
{
    char *result;
    int len;

    if (!str)
        return NULL;

    len = ft_strlen(str);
    result = malloc(len + 2);
    if (!result)
        return NULL;

    ft_strcpy(result, str);
    result[len] = c;
    result[len + 1] = '\0';

    return result;
}

char *expansion(char *str, int *j, t_data *data, t_quotes *inside_quotes)
{
    t_env *env = data->envp_list;
    char *variable;
    char *result;
    int k = *j + 1;

    if (str[k] == ' ' || str[k] == '\0' || str[k] == '$' || str[k] == '\'' || str[k] == '\"')
    {
        (*j)++;
        return ft_strdup("$");
    }
    while (str[k] != ' ' && str[k] != '$' && str[k] != '\0' && str[k] != '\'' && str[k] != '\"')
        k++;
    variable = ft_substr(str, *j + 1, k - *j - 1);
    if (*variable == '?')
    {
        result = ft_itoa(data->exit_status);
        free(variable);
        *j = k;
        return result;
    }
    while (env)
    {
        if (ft_strcmp(variable, env->key) == 0)
        {
            result = ft_strdup(env->value);
            free(variable);
            *j = k;
            return result;
        }
        env = env->next;
    }

    free(variable);
    *j = k;
    return ft_strdup("");
}

void expand_variables(t_cmd **cmd, t_data *data)
{
    int i;
    char *expanded;
    t_quotes inside_quotes;
    int j = 0;
    while ((*cmd)->content[j])
    {
        char *str = (*cmd)->content[j];
        char *result = ft_strdup("");
        char *temp;

        initialize_quote(&inside_quotes);
        i = 0;
        while (str[i])
        {
            update_quotes(&inside_quotes, str[i]);

            if (str[i] == '$' && inside_quotes.single_quote == 0)
            {
                int start = i;
                expanded = expansion(str, &i, data, &inside_quotes);
                temp = ft_strjoin(result, expanded);
                free(result);
                result = temp;
                free(expanded);
            }
            else
            {
                temp = ft_strjoinchar(result, str[i]);
                free(result);
                result = temp;
                i++;
            }
        }
        free((*cmd)->content[j]);
        (*cmd)->content[j] = result;
        j++;
    }
} 

