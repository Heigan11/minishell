/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 19:18:33 by chilton           #+#    #+#             */
/*   Updated: 2021/05/14 19:25:24 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipes(int ***fd, size_t size)
{
	size_t	j;

	*fd = (int **)malloc(sizeof(int *) * (size - 1));
	if (!*fd)
		exit_error("init_pid_fd() malloc()");
	j = 0;
	while (j < (size - 1))
	{
		(*fd)[j] = malloc(sizeof(int) * 2);
		if (!(*fd)[j])
			exit_error("init_pid_fd() malloc()");
		if (pipe((*fd)[j]) == -1)
			exit_error("pipe() in init_pid_fd() malloc()");
		j++;
	}
}

void	free_pipes(int **fd, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size - 1)
	{
		free(fd[i]);
		i++;
	}
	free(fd);
}

void	close_pipes(int **fd, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
}

void	dup_fd(size_t i, int **pipes_fd, size_t size)
{
	if (i != size - 1)
		if (dup2(pipes_fd[i][1], STDOUT_FILENO) == -1)
			exit_error("dup_fd(.., STDOUT_FILENO) dup2()");
	if (i != 0)
		if (dup2(pipes_fd[i - 1][0], STDIN_FILENO) == -1)
			exit_error("dup_fd(.., STDIN_FILENO) dup2()");
	close_pipes(pipes_fd, size - 1);
}
