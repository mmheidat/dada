#include <stdio.h>
#include <stdlib.h>   
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>

// child writes - parent reads

int ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return(i);
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	int pid;
	int fdpipes[2];
	if (pipe(fdpipes) == -1)
		return(1);
	pid = fork();
	if (pid == -1)
		return(1);
	if (pid == 0)
	{
		close(fdpipes[0]);
		char *message = "what am I doing";
		write(fdpipes[1], message, ft_strlen(message) + 1);
		close(fdpipes[1]);
	}
	else
	{
		close(fdpipes[1]);
		char buffer[100];
		read(fdpipes[0], buffer, sizeof(buffer));
		printf("received from child ===== %s\n", buffer);
		close(fdpipes[0]);
		wait(NULL);
	}
}


// int main(int argc, char **argv)
// {
// 	(void)argc;
// 	(void)argv;
// 	int fd[2];
// 	// fd[0] - read;
// 	// fd[1] - write;
// 	if (pipe(fd) == -1)
// 	{
// 		perror("pipe");
// 		exit(1);
// 	}
// 	int id = fork();
// 	if (id == -1)
// 	{
// 		perror("fork");
// 		exit(1);
// 	}
// 	if (id == 0)
// 	{
// 		// the child process is gonna write data, so we close fd[0] because it's for reading not writing data.
// 		close(fd[0]);
// 		int x;
// 		printf("Input num; ");
// 		scanf("%d", &x);
// 		write(fd[1], &x, sizeof(int));
// 		close(fd[1]);
// 	}
// 	else
// 	{
// 		// the parent process is gonna read data, so we close fd[1] because it's for writing not reading data.
// 		close(fd[1]);
// 		int y;
// 		read(fd[0], &y, sizeof(int));
// 		close(fd[0]);
// 		printf("Number = %d\n", y);
// 		wait(NULL);
// 	}
// }