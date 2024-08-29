/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmheidat <mmheidat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 03:26:05 by mbamatra          #+#    #+#             */
/*   Updated: 2024/08/29 16:26:28 by mmheidat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void	sort_array(t_env **export_array, int size)
{
	int i;
	int j;
	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(export_array[j]->key, export_array[j + 1]->key) > 0)
				swap(export_array[j], export_array[j + 1]);
			j++;
		}
		i++;
	}
}

t_env **list_array(t_env *env_list, int *size) 
{
    t_env *current = env_list;
    t_env **array;
    int i = 0;
    *size = 0;
    while (current) 
	{
        (*size)++;
        current = current->next;
    }
    array = malloc(sizeof(t_env *) * (*size));
    if (!array)
        return NULL;
    current = env_list;
    while (current) 
	{
        array[i++] = current;
        current = current->next;
    }
    return array;
}


void	swap(t_env *a, t_env *b)
{
	char *tmp_key;
	char *tmp_value;
	tmp_key = a->key;
	tmp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = tmp_key;
	b->value = tmp_value;
}

void print_env(t_env *env_list)
{
    t_env *current = env_list;

    while (current)
    {
        if (current->value)
            printf("%s=%s\n", current->key, current->value);
        else
            printf("%s\n", current->key);
        current = current->next;
    }
}

void sort_and_print_export(t_env *env_list) 
{
    int size;
    t_env **export_array;

	if (!env_list)
		return ;
	export_array = list_array(env_list, &size);
    if (export_array) 
    {
        sort_array(export_array, size);
        print_export(*export_array);
        free(export_array);
    } 
    else 
        printf("Error\n");
}

void sort_export(t_env *env_list) 
{
    int size;
    t_env **export_array;
    export_array = list_array(env_list, &size);
    if (export_array) 
	{
        sort_array(export_array, size);
        free(export_array);
    } 
	else 
        printf("Error\n");
}

void print_export(t_env *export_list) 
{
    t_env *current = export_list;
    while (current) 
    {
        if (current->value)
            printf("declare -x %s=\"%s\"\n", current->key, current->value);
        else
            printf("declare -x %s\n", current->key);
        current = current->next;
    }
}

void export(t_data *data, t_cmd *token) 
{
    char *key;
    char *value;
    char *delimiter;
    int i = 1;

    if (!token->content[1]) 
    {
        data->exit_status = 0;
        sort_and_print_export(data->export_list);
        return;
    }

    while (token->content[i])
    {
        delimiter = ft_strchr(token->content[i], '=');
        if (delimiter) 
        {
            *delimiter = '\0';
            key = ft_strdup(token->content[i]);
            printf("key = %d", key[0]);
			if ((!ft_isalpha(key[0]) && key[0] != '_'))
			{
				printf("bash: export: `%s': not a valid identifier\n", key);
                data->exit_status = 1;
				free(key);
				i++;
				return;
			}
            value = ft_strdup(delimiter + 1);
            if (key && value) 
            {
                add_env(&(data->export_list), key, value);
                add_env(&(data->envp_list), key, value);
                free(key);
                free(value);
            }
        } 
        else 
        {
            key = ft_strdup(token->content[i]);
            if (key && !ft_isalpha(key[0]) && key[0] != '_')
            {
				printf("bash: export: `%s': not a valid identifier\n", key);
                data->exit_status = 1;
				i++;
                free(key);
				return;
            }
        }
        i++;
    }
    data->exit_status = 0;
}




void	remove_env(t_env **env_list, t_cmd *token)
{
	t_env *current;
	t_env *previous;
    int i = 1;
	current = *env_list;
	previous = NULL;
	if (!token->content[1])
		return ;
	while (current)
	{
		if (ft_strcmp(current->key, token->content[i]) == 0)
		{
			if (previous)
				previous->next = current->next;
			else
				*env_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}
int	check_key(t_env *current, char *key, char *value)
{
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (current->value)
				free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			return (1);
		}
		current = current->next;
	}
	return (0);
}

void add_env(t_env **env_list, char *key, char *value)
{
    t_env *new_env;
    t_env *current;

	current = *env_list;
	if (check_key(current, key, value))
		return ;
    new_env = malloc(sizeof(t_env));
    if (!new_env)
        return;
    new_env->key = ft_strdup(key);
    new_env->value = ft_strdup(value);
    new_env->next = NULL;
    if (*env_list == NULL)
        *env_list = new_env;
    else
    {
        current = *env_list;
        while (current->next != NULL)
            current = current->next;
        current->next = new_env;
    }
}

void	init_env(t_data *data, char **envp)
{
	int i = 0;
	char *envp_key;
	char *delimiter;
	while (envp[i])
	{
		envp_key = ft_strdup(envp[i]);
		delimiter = ft_strchr(envp_key, '=');
		if (delimiter)
		{
			*delimiter = '\0';
			add_env(&data->envp_list, envp_key, delimiter + 1);
			add_env(&data->export_list, envp_key, delimiter + 1);
		}
		free(envp_key);
		i++;
	}
}

void	data_init(t_data *data)
{
	data->line = NULL;
	data->envp = NULL;
    data->exit_status = 0;
	// data->export = NULL;
	data->envp_list = NULL;
	data->export_list = NULL;
	data->path = NULL;
	// data->cmd = NULL;
	data->tokens = NULL;
}

void	free_tokens2(t_cmd **head)
{
	t_cmd	*tmp;
    int i = 0;

	while (*head)
	{
		tmp = *head;
    	*head = (*head)->next;
        while (tmp->content[i])
        	free(tmp->content[i++]);
        free(tmp->content);
    	free(tmp);
	}
}

void add_temp_env(t_data *data, t_cmd *token)
{
	if (!token->content[1])
	{
		print_env(data->envp_list) ;
		return ;
	}
    char *delimiter;
	delimiter = ft_strchr(token->content[1], '=');
	// if (!delimiter)
	// {
	// 	printf("bash: env: %s: No such file or directory\n", token->content[1]);
	// 	return ;
	// }
	*delimiter = '\0';
	add_env(&data->envp_list, token->content[1], delimiter + 1);
	print_env(data->envp_list);
	return ;
}

int check_current(t_cmd *cmd, t_data data)
{
	if (!cmd || !cmd->content || !cmd->content[0])
        return (printf("bash: command not found\n"), 1);
    if (ft_strcmp(cmd->content[0], "exit") == 0)
        printf("exit\n"), exit(0);
    else if (ft_strcmp(cmd->content[0], "export") == 0) 
        return(export(&data, cmd), 0);
    else if (ft_strcmp(cmd->content[0], "clear") == 0)
        return(system("clear"), 0);
    else if (ft_strcmp(cmd->content[0], "env") == 0)
        return (add_temp_env(&data, cmd), 0);
	else if (ft_strcmp(cmd->content[0], "cd") == 0)
        return (cd(cmd, &data), 0);
	else if (ft_strcmp(cmd->content[0], "echo") == 0)
        return (ft_echo(cmd->content), 0);
	else if (ft_strcmp(cmd->content[0], "pwd") == 0)
		return (ft_pwd(), 0);
    else if (ft_strcmp(cmd->content[0], "unset") == 0)
        return(remove_env(&data.export_list, cmd), remove_env(&data.envp_list, cmd), 0);
	return (1);
}
void handle_sigquit(int sig)
{
    (void)sig;  // We don't need the signal number
}

void handle_sigint(int sig)
{
    (void)sig;
    // Print a new line to move to the next line
    write(1, "\n", 1);
    // Re-display the prompt (optional: this depends on how you manage the prompt)
    rl_on_new_line();
    // rl_replace_line("", 0);
    rl_redisplay();
}

void execute_pipes(t_cmd *cmd, char **envp, t_data *data)
{
    int pipe_fds[2];
    pid_t pid;
    t_cmd *current_cmd = cmd;
    int fd_in = -1; // Input file descriptor
    
    while (current_cmd)
    {
        if (current_cmd->next) // If not the last command
        {
            if (pipe(pipe_fds) == -1) // Create pipe
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        
        if (pid == 0)
        {
            if (fd_in != -1)
            {
                dup2(fd_in, STDIN_FILENO); // Redirect input
                close(fd_in);
            }
            if (current_cmd->next)
            {
                dup2(pipe_fds[1], STDOUT_FILENO); // Redirect output
                close(pipe_fds[1]);
            }
            else
            {
                // If it's the last command, just close the pipe
                close(pipe_fds[1]);
            }
            
            handle_redirection(current_cmd->files);
            
            if (execute_command(current_cmd->files, current_cmd->content, envp, data->envp_list) != 0)
                exit(EXIT_FAILURE);
            
            exit(EXIT_SUCCESS);
        }
        else
        {
            waitpid(pid, NULL, 0); // Wait for the child process to finish
            
            if (fd_in != -1)
                close(fd_in); // Close the previous input file descriptor
            
            if (current_cmd->next)
                close(pipe_fds[1]); // Close the write end of the pipe
            
            fd_in = pipe_fds[0]; // Save the read end of the pipe for the next command
            
            current_cmd = current_cmd->next; // Move to the next command
        }
    }
}
void handle_eof(int signum) 
{
    (void)signum; // Ignore the signal number
    printf("exit\n");
    exit(0);
}

int main(int ac, char **av, char **envp)
{
    (void)ac;
    (void)av;
    t_cmd *cmd;
    char    **split;
    t_data data;
	signal(SIGQUIT, handle_sigquit);
	signal(SIGINT, handle_sigint);
	signal(SIGTERM, handle_eof); 
    data_init(&data);
    init_env(&data, envp);
    while (1)
    {
        data.line = readline("minishell$ ");
        if (!data.line)
            exit(0);
        if (!*data.line)
            continue;
        initialize_quote(&data.quotes);
        add_history(data.line);
        if (parsing(data.line) == 1)
        {
            write(2, "Syntax Error\n", 13);
            data.exit_status = 258;
            free(data.line);
            continue;
        }
        data.line = join_spaces(data.line);
		// starting_parsing(data.line, &head);
		// current = head;
        split = ft_split_without_quotes(data.line, '|');
        cmd = fill_command(split);
        expand_variables(&cmd, &data);
        // print_cmd(cmd);
		do_exclude_quotes(cmd);
		if (cmd && cmd->next) // Check if there's a pipe
        {
            execute_pipes(cmd, envp, &data);
        }
		else if (check_current(cmd, data) == 1)
	    {
            if (execute_command(cmd->files, cmd->content, envp, data.envp_list) != 0)
                data.exit_status = 1;
            else
                data.exit_status = 0;
        }
        free(data.line);
    }
}