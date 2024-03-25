/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fchieric <fchieric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:23:48 by fchieric          #+#    #+#             */
/*   Updated: 2024/03/25 18:03:15 by fchieric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char *cmd, char **envp)
{
	char	**all_paths;
	char	*path;
	int		i;
	char	*path_cmd;

	i = 0;
	while (ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	all_paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (all_paths[i++])
	{
		path = ft_strjoin(all_paths[i], "/");
		path_cmd = ft_strjoin(path, cmd);
		free(path);
		if (access(path_cmd, F_OK) == 0)
		{
			free_matrix(all_paths);
			return (path_cmd);
		}
		free(path_cmd);
	}
	free_matrix(all_paths);
	error_cmd(cmd);
	return (0);
}

void	exec_cmd(char *argv, char **envp)
{
	char	**cmd_matrix;
	char	*path;

	cmd_matrix = ft_split(argv, ' ');
	path = find_path(cmd_matrix[0], envp);
	if (!path)
	{
		free_matrix(cmd_matrix);
		return ;
	}
	if (execve(path, cmd_matrix, envp) == -1)
	{
		write(2, "Error: impossible execution\n", 28);
		return ;
	}
}

void	proc_child(char **argv, char **envp, int *fd)
{
	int	infile;

	infile = open(argv[1], O_RDONLY, 0777);
	if (infile == -1)
	{
		write(2, "Error: impossible execution\n", 28);
		return ;
	}
	dup2(fd[1], STDOUT_FILENO);
	dup2(infile, STDIN_FILENO);
	close(fd[0]);
	exec_cmd(argv[2], envp);
}

void	proc_parent(char **argv, char **envp, int *fd)
{
	int	outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
	{
		write(2, "Error: impossible execution\n", 28);
		return ;
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(fd[1]);
	exec_cmd(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid;

	if (argc == 5)
	{
		if (pipe(fd) == -1)
			perror("Error: impossible execution\n");
		pid = fork();
		if (pid == -1)
			perror("Error: impossible execution\n");
		if (pid == 0)
			proc_child(argv, envp, fd);
		waitpid(pid, NULL, 0);
		proc_parent(argv, envp, fd);
	}
	else
	{
		write(2, "Error: wrong number of arguments\n", 33);
		write(2, "Try: file1 cmd1 cmd2 file2\n", 27);
		return (1);
	}
	return (0);
}
