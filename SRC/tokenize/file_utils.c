/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbamatra <mbamatra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 06:47:32 by mbamatra          #+#    #+#             */
/*   Updated: 2024/08/22 19:37:17 by mbamatra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

t_files	*ft_file_lstnew(char *file_name, char *type)
{
	t_files	*new_file;

	new_file = malloc(sizeof(t_files));
	if (!new_file)
		return (NULL);
	new_file->file_name = file_name;
	if (type[0] == '>')
	{
		if (type[1] == '>')
			new_file->type = APPEND; /// append
		else
			new_file->type = REDIRECT_IN; /// redirect infile
	}
	else if (type[0] == '<')
	{
		if (type[1] == '<')
			new_file->type = HEREDOC; /// heredoc
		else
			new_file->type = REDIRECT_OUT; /// redirect outfile
	}
	new_file->next = NULL;
	return (new_file);
}

void ft_files_lstaddback(t_files **files, t_files *new_file)
{
	t_files	*last;

	if (!files || !new_file)
		return ;
	if (!*files)
	{
		*files = new_file;
		return ;
	}
	last = *files;
	while (last->next)
		last = last->next;
	last->next = new_file;
}

