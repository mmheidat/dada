/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmheidat <mmheidat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 11:34:56 by mmheidat          #+#    #+#             */
/*   Updated: 2024/08/29 11:35:33 by mmheidat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

void ft_echo(char **args)
{
    int i = 1;
    int print_newline = 1; // Default behavior is to print a newline

    if (args[i] && strcmp(args[i], "-n") == 0) {
        print_newline = 0; // No newline
        i++;
    }

    for (; args[i]; i++) {
        printf("%s", args[i]);
        if (args[i + 1]) {
            printf(" ");
        }
    }
    if (print_newline) {
        printf("\n");
    }
}