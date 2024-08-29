/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmheidat <mmheidat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:55:36 by mbamatra          #+#    #+#             */
/*   Updated: 2024/08/28 20:26:21 by mmheidat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void print_double(char **double_pointer)
{
	int i = 0;
	if (!double_pointer)
		return ;
	while (double_pointer[i])
	{
		printf("cmd[%d]: (%s)\n", i, double_pointer[i]);
		i++;
	}
}
// void print_files(t_files *files)
// {
// 	while (files)
// 	{
// 		printf("file: %s ------ type: %d\n", files->file_name, files->type);
// 		files = files->next;
// 	}
// 	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
// }

void print_cmd(t_cmd *cmd)
{
	while (cmd)
	{
		// print_files(cmd->files);
		print_double(cmd->content);
		cmd = cmd->next;
	}
}

int size_of_content(char **splitted)
{
	int i = 0;
	int size = 0;
	while (splitted[i])
	{
		if (splitted[i][0] == '>' || splitted[i][0] == '<')
			i++;
		else
			size++;
		i++;
	}
	return (size);
}

t_cmd *fill_command(char **split)
{
	int 	i;
	int 	j;
	int 	n;
	t_cmd	*cmd;
	t_files	*files;
	char **splitted;
	char **the_cmd;

	files = NULL;
	cmd = NULL;
	i = 0;
	if (!split)
		return (NULL);
	while (split[i])
	{
		files = NULL;
		splitted = ft_split_without_quotes(split[i], ' ');
		the_cmd = (char **)malloc(sizeof(char *) * (size_of_content(splitted) + 1));
		if (!the_cmd)
			return (NULL);
		j = 0;
		n = 0;
		while (splitted[j])
		{
			if (splitted[j][0] == '>' || splitted[j][0] == '<')
			{
				ft_files_lstaddback(&files, ft_file_lstnew(splitted[j + 1], splitted[j]));
				j++;
			}
			else
			{
				the_cmd[n] = splitted[j];
				n++;
			}
			j++;
		}
		the_cmd[n] = NULL;
		ft_cmd_lstadd_back(&cmd, ft_cmd_lstnew(the_cmd, files));
		i++;
	}
	return (cmd);
}
