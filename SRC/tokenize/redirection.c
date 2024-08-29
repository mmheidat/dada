/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmheidat <mmheidat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 08:54:47 by mbamatra          #+#    #+#             */
/*   Updated: 2024/08/29 08:44:43 by mmheidat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void handle_redirection(t_files *files)
{
	int fd;
	t_files *tmp;
	tmp = files;
	while (files)
	{
		if (files->type == REDIRECT_IN)
		{
			fd = open(files->file_name, O_CREAT | O_WRONLY, 0644);
			dup2(fd, 1);
		}
		else if (files->type == APPEND)
		{
			fd = open(files->file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
			dup2(fd, 1);
		}
		else if (files->type == REDIRECT_OUT)
		{
			fd = open(files->file_name, O_RDONLY, 0644);
			dup2(fd, 0);
		}
		else if (files->type == HEREDOC)
		{
			handle_heredoc(files);
		}

		if (fd < 0 && files->type != HEREDOC)
		{
			write(2, "Error opening file.\n", 20);
			exit(1);
		}
		close(fd);
		files = files->next;
	}
}

void handle_heredoc(t_files *files)
{
	char *line;
	int pipefd[2];

	if (pipe(pipefd) == -1) 
	{
		perror("pipe");
		exit(1);
	}
	while (1)
	{
		line = readline("> ");
		if (line == NULL || strcmp(line, files->file_name) == 0)
		{
			free(line);
			break;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	dup2(pipefd[0], 0);
	close(pipefd[0]);
}

char **get_path(t_env *envp_list)
{
	char *path = NULL;
	char **path_split;
	int i;
	t_env *tmp;
	tmp = envp_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "PATH") == 0)
		{
			path = ft_strdup(tmp->value);
			break;
		}
		tmp = tmp->next;
	}
	if (path)
	{
		path_split = ft_split_without_quotes(path, ':');
		free(path);
		return (path_split);
	}
	return (NULL);
}

int execute_command(t_files *files, char **command, char **envp, t_env *envp_list)
{
	pid_t pid;
	char *tmp_cmd;
	char **path;
	pid = fork();
	if (pid == -1)
		exit(1);
	if (pid == 0)
	{
		handle_redirection(files);
		if (!command || !command[0])
			exit(0);
		if (execve(command[0], command, envp) == -1) /// /bin/ls
		{
			path = get_path(envp_list);
			if (path)
			{
				tmp_cmd = command[0];
				while (*path)
				{
					tmp_cmd = ft_strjoin(ft_strjoin(*path, "/"), tmp_cmd);
					if (execve(tmp_cmd, command, envp) != -1)
						break;
					free(tmp_cmd);
					tmp_cmd = command[0];
					path++;
				}
			}
			write(2, command[0], ft_strlen(command[0]));
			write(2, ": command not found\n", 21);
			exit(127);
		}
	}
	else
		waitpid(pid, NULL, 0);
	return (0);
}

// > = redirect_in // write to
// < = redirect_out // read from
// >> = append
// << = heredoc

// void handle_redirection(t_files *files)
// {
// 	int fd;
// 	t_files *tmp;
// 	tmp = files;
// 	while (files)
// 	{
// 		if (files->type == REDIRECT_IN)
// 		{
// 			fd = open(files->file_name, O_CREAT | O_WRONLY, 0644);
// 			dup2(fd, 1);
// 		}
// 		else if (files->type == APPEND)
// 		{
// 			fd = open(files->file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
// 			dup2(fd, 1);
// 		}
// 		else if (files->type == REDIRECT_OUT)
// 		{
// 			fd = open(files->file_name, O_RDONLY, 0644);
// 			dup2(fd, 0);
// 		}
// 		if (fd < 0)
// 			(write(2, "Error opening file.\n", 20), exit(1));
// 		close(fd);
// 		// else if (files->type == HEREDOC)
// 		// {
// 			// heredoc(files->file_name);
// 		// }
// 		files = files->next;
// 	}
// }

// void execute_command(t_files *files, char **command)
// {
// 	pid_t pid;
// 	pid = fork();
// 	if (pid == -1)
// 		exit(1);
// 	if (pid == 0)
// 	{
// 		if (!command || !command[0])
// 			exit(0);
// 		handle_redirection(files);
// 		if (execvp(command[0], command) == -1)
// 		{
// 			// printf("command[0] = %s", command[0]);
// 			write(2, command[0], ft_strlen(command[0]));
// 			write(2, ": is a directory.\n", 18);
// 			exit(1);
// 		}
// 	}
// 	else
// 		waitpid(pid, NULL, 0);
// }

// Why dup2 is Essential for Redirections:
// When you perform input or output redirection, you need to redirect the standard input/output (stdin/stdout) of a process to a file or another file descriptor. dup2 plays a crucial role in this redirection process.

// How dup2 Works:
// dup2(oldfd, newfd): This function duplicates the file descriptor oldfd to newfd. If newfd is already open, it is first closed before being duplicated.
// After dup2, newfd becomes an exact copy of oldfd, meaning they both point to the same open file description, and the next read or write operation on newfd will affect the file that oldfd was pointing to.
// Usage in Redirection:
// Output Redirection (>):

// You want the standard output (stdout, typically file descriptor 1) to go to a file instead of the terminal.
// You open the file with open(), which gives you a file descriptor.
// Use dup2(fd, STDOUT_FILENO) to make the file descriptor replace stdout, so all subsequent output will be written to the file.
// Input Redirection (<):

// You want the standard input (stdin, typically file descriptor 0) to come from a file instead of the keyboard.
// You open the file with open(), which gives you a file descriptor.
// Use dup2(fd, STDIN_FILENO) to make the file descriptor replace stdin, so all subsequent input will come from the file.
// Example Scenario:
// Suppose you want to execute a command like cat < input.txt > output.txt. Here's how dup2 would be used:

// Redirecting Input:
// Open input.txt for reading: fd_in = open("input.txt", O_RDONLY);
// Redirect stdin to input.txt: dup2(fd_in, STDIN_FILENO);
// Redirecting Output:
// Open output.txt for writing (and truncating): fd_out = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
// Redirect stdout to output.txt: dup2(fd_out, STDOUT_FILENO);
// After setting up these redirections, when the command is executed (e.g., execvp("cat", args);), the cat command reads from input.txt and writes to output.txt instead of the terminal.

// Conclusion:
// dup2 is essential for redirections in Minishell because it allows you to change the file descriptors for stdin, stdout, or stderr to point to files, pipes, or other processes. Without dup2, you wouldn't be able to implement
// redirections properly, which are a core feature of any shell.