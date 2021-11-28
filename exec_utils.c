/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 19:02:46 by Jraye             #+#    #+#             */
/*   Updated: 2021/05/19 18:11:54 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin(char **args, t_shell *sh)
{
	if (args == NULL)
		return (1);
	else if (!ft_strcmp(*args, "echo"))
		ft_echo(args + 1, &sh->status);
	else if (!ft_strcmp(*args, "cd"))
		ft_cd(args + 1, sh->env_list, &sh->status);
	else if (!ft_strcmp(*args, "pwd"))
		ft_pwd(&sh->status);
	else if (!ft_strcmp(*args, "export"))
		ft_export(args + 1, &sh->env_list, &sh->status);
	else if (!ft_strcmp(*args, "unset"))
		ft_unset(args + 1, &sh->env_list, &sh->status);
	else if (!ft_strcmp(*args, "env"))
		ft_env(args + 1, sh->env_list, sh->cmd_last_arg, &sh->status);
	else if (!ft_strcmp(*args, "exit"))
		ft_exit(args + 1, sh);
	else
		return (0);
	return (1);
}

void	exec_cmd(char *cmd_path, char **args, t_list *env_list, char *last_arg)
{
	char	**env;

	env = env_list_to_table(env_list, last_arg);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	execve(cmd_path, args, env);
	ft_splitfree(env);
	free(cmd_path);
	error_msg(args[0], strerror(errno));
	exit (126);
}

int	get_exit_status(int exit)
{
	if (WIFEXITED(exit))
		return (WEXITSTATUS(exit));
	else if (WIFSIGNALED(exit))
	{
		exit = WTERMSIG(exit);
		if (exit == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (exit == SIGQUIT)
			printf("Quit: %d\n", exit);
		else if (exit == SIGTERM)
			printf("🛑Terminated: %d\n", exit);
		else if (exit == SIGKILL)
			printf("💀Killed: %d\n", exit);
		return (exit + 128);
	}
	else
		return (exit);
}

char	*get_last_arg(char **args)
{
	if (args == NULL)
		return (NULL);
	while (args[1])
		args++;
	return (*args);
}

void	set_last_arg(char **last_arg, char *arg)
{
	free(*last_arg);
	if (arg == NULL)
		*last_arg = NULL;
	else
	{
		*last_arg = ft_strdup(arg);
		if (!*last_arg)
			exit_error("ft_strdup() in set_last_arg() malloc()");
	}
}
