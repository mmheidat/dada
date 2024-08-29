/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmheidat <mmheidat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 19:43:29 by mmheidat          #+#    #+#             */
/*   Updated: 2024/08/28 20:15:04 by mmheidat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"


void cd(t_cmd *cmd, t_data *data) {
    char *dir;
    char resolved_path[MAX_PATH_LEN];

    // Check if the user provided a directory
    if (cmd->content[1] == NULL) {
        // If no directory is provided, default to the home directory
        dir = getenv("HOME");
        if (dir == NULL) {
            perror("cd: HOME not set");
            data->exit_status = 1;
            return;
        }
    } else {
        dir = cmd->content[1];
    }

    // Resolve the directory path
    if (resolve_directory(dir, resolved_path) != 0) {
        perror("cd: directory resolution error");
        data->exit_status = 1;
        return;
    }

    // Change the directory
    if (chdir(resolved_path) != 0) {
        perror("cd");
        data->exit_status = 1;
    } else {
        data->exit_status = 0;
    }
}

int resolve_directory(const char *path, char *resolved_path) {
    // Check for symbolic links and resolve the absolute path
    if (realpath(path, resolved_path) == NULL) {
        return -1; // Error in resolving the directory
    }
    return 0; // Successfully resolved
}