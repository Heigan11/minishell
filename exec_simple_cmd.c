/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 19:42:13 by chilton           #+#    #+#             */
/*   Updated: 2021/05/19 18:12:21 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	save_std(t_redirect **redirects, int std_fd[2])
{
	if (redirects)
	{
		std_fd[0] = dup(STDIN_FILENO);
		if (std_fd[0] == -1)
			exit_error("dup(STDIN_FILENO) in save_std");
		std_fd[1] = dup(STDOUT_FILENO);
		if (std_fd[1] == -1)
			exit_error("dup(STDOUT_FILENO) in save_std");
	}
}

static void	restore_std(t_redirect **redirects, int std_fd[2])
{
	if (redirects)
	{
		if (dup2(std_fd[0], STDIN_FILENO) == -1)
			exit_error("dup2(.., STDIN_FILENO) in restore_std");
		close(std_fd[0]);
		if (dup2(std_fd[1], STDOUT_FILENO) == -1)
			exit_error("dup2(.., STDOUT_FILENO) in restore_std");
		close(std_fd[1]);
	}
}

static void	exec_fork(char *cmd_path, char **args,
			t_list *env_list, char *last_cmd_arg)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit_error("exec_cmd() fork()");
	if (pid == 0)
		exec_cmd(cmd_path, args, env_list, last_cmd_arg);
}

void	exec_simple_cmd(t_command *cmd, t_shell *sh)
{
	char	*cmd_path;
	int		std_fd[2];

	set_last_arg(&sh->cmd_last_arg, get_last_arg(cmd->args));
	save_std(cmd->redirs, std_fd);
	if (make_redirects(cmd->redirs, sh))
	{
		if (exec_builtin(cmd->args, sh) == 0)
		{
			cmd_path = get_cmd_path(cmd->args[0], sh->env_list, &sh->status);
			if (cmd_path)
			{
				exec_fork(cmd_path, cmd->args, sh->env_list, sh->cmd_last_arg);
				wait(&sh->status);
				sh->status = get_exit_status(sh->status);
				free(cmd_path);
			}
		}
	}
	restore_std(cmd->redirs, std_fd);
}
