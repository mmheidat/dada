/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmheidat <mmheidat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 03:21:11 by mbamatra          #+#    #+#             */
/*   Updated: 2024/08/29 11:36:55 by mmheidat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL_H
#define MINI_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <termios.h>
#include "../libft/libft.h"
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

#define APPEND 3
#define REDIRECT_IN 0
#define HEREDOC 2
#define REDIRECT_OUT 1
#define MAX_PATH_LEN 1024 

typedef struct cmd {
	char **content;
	struct cmd *next;
	struct t_files *files;
	////redirections
}	t_cmd;

typedef struct t_files {
	char *file_name;
	int type;
	struct t_files *next;
}	t_files;

typedef struct s_env {
	char *key;
	char *value;
	struct s_env *next;
}	t_env;

typedef struct s_quotes {
	int	single_quote;
	int	double_quote;
}	t_quotes;

typedef struct s_data
{
	char	*line;
	char	**envp;
	// char	**export;
	t_env	*envp_list;
	t_env	*export_list;
	char	**path;
	// char	**cmd;
	int exit_status;
	char	**tokens;
	char *(*get_dir)(int argc, char **argv);
	t_quotes quotes;
	t_files *files;
}	t_data;

int	validate_direction(char *str, int i);
int	check_redirection(char *str);
int	check_quotes(char *str);
int	check_pipes(char *str);
int	parsing(char *str);
void	update_quotes(t_quotes *inside_quotes, char c);
void 	initialize_quote(t_quotes *inside_quotes);
void	add_env(t_env **env_list, char *key, char *value);
void	print_env(t_env *env_list);
void	export(t_data *data, t_cmd *token);
void	remove_env(t_env **env_list, t_cmd *token);
void	init_env(t_data *data, char **envp);
void	data_init(t_data *data);
void	sort_array(t_env **env_array, int size);
t_env **list_array(t_env *env_list, int *size);
void	swap(t_env *a, t_env *b);
void	print_env(t_env *env_list);
void	print_export(t_env *export_list); 
void	sort_and_print_export(t_env *env_list);
void	add_token(t_cmd **head, char *str);
void	print_cmds(t_cmd *token);
int		parsing(char *str);
int		validate_direction(char *str, int i);
int		check_redirection(char *str);
int		check_quotes(char *str);
int execute_command(t_files *files, char **command, char **envp, t_env *env_list);
void	ft_cmd_lstadd_back(t_cmd **lst, t_cmd *new);
t_cmd	*ft_cmd_lstlast(t_cmd *lst);
t_cmd	*ft_cmd_lstnew(char **content, t_files *files);
int		ft_cmd_lstsize(t_cmd *lst);
t_cmd	*fill_command(char **split);
void	print_cmd(t_cmd *cmd);
char	*join_spaces(char *line);
char	**ft_split_without_quotes(char const *s, char c);
t_files	*ft_file_lstnew(char *file_name, char *type);
void ft_files_lstaddback(t_files **files, t_files *new_file);
char *expansion(char *str, int *j, t_data *data, t_quotes *inside_quotes);
void handle_heredoc(t_files *files);
void expand_variables(t_cmd **cmd, t_data *data);
void do_exclude_quotes(t_cmd *command);
void cd(t_cmd *cmd, t_data *data);
int resolve_directory(const char *path, char *resolved_path);
int is_symlink(const char *path);
void handle_redirection(t_files *files);
void ft_echo(char **args);
void ft_pwd(void);

#endif