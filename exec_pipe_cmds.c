/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_cmds.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 17:23:06 by chilton           #+#    #+#             */
/*   Updated: 2021/05/19 18:12:37 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	wait_all_processes(size_t size, pid_t pid, int *status)
{
	size_t	i;
	int		cur_status;

	i = 0;
	while (i < size)
	{
		if (wait(&cur_status) == pid)
			*status = get_exit_status(cur_status);
		else if (WIFEXITED(cur_status) == 0 && \
				WIFSIGNALED(cur_status) && \
				WTERMSIG(cur_status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

static void	exec_in_fork(t_command	*cmd, t_shell *sh)
{
	char	*cmd_path;

	if (make_redirects(cmd->redirs, sh) == 0)
		exit(sh->status);
	if (exec_builtin(cmd->args, sh) == 1)
		exit(sh->status);
	cmd_path = get_cmd_path(cmd->args[0], sh->env_list, &sh->status);
	if (!cmd_path)
		exit(sh->status);
	exec_cmd(cmd_path, cmd->args, sh->env_list, sh->cmd_last_arg);
}

void	exec_pipe_cmds(t_command **cmds, size_t size, t_shell *sh)
{
	int		**pipes_fd;
	size_t	i;
	pid_t	pid;

	init_pipes(&pipes_fd, size);
	i = 0;
	while (i < size)
	{
		set_last_arg(&sh->cmd_last_arg, get_last_arg(cmds[i]->args));
		pid = fork();
		if (pid == -1)
			exit_error("exec_pipe_cmds() fork()");
		if (pid == 0)
		{
			dup_fd(i, pipes_fd, size);
			exec_in_fork(cmds[i], sh);
		}
		i++;
	}
	set_last_arg(&sh->cmd_last_arg, NULL);
	close_pipes(pipes_fd, size - 1);
	wait_all_processes(size, pid, &sh->status);
	free_pipes(pipes_fd, size);
}
