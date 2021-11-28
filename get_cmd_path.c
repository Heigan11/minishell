/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/05 19:32:24 by chilton           #+#    #+#             */
/*   Updated: 2021/05/14 21:39:57 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "minishell.h"

static int	is_path_valid(char *cmd_path, int *status)
{
	struct stat	info;

	if (cmd_path == NULL)
		return (0);
	if (stat(cmd_path, &info) == 0)
	{
		if (S_ISDIR(info.st_mode))
		{
			*status = 126;
			error_msg(cmd_path, "is a directory");
			return (0);
		}
		return (1);
	}
	*status = 127;
	error_msg(cmd_path, strerror(errno));
	return (0);
}

static char	*find_cmd_in_path(char *cmd, const char *path)
{
	char		**path_table;
	char		*cmd_path;
	struct stat	info;
	size_t		i;

	if (!path)
		return (NULL);
	path_table = ft_split(path, ':');
	if (!path_table)
		exit_error("ft_split() in get_cmd_path() malloc()");
	i = 0;
	cmd_path = NULL;
	while (path_table[i])
	{
		cmd_path = ft_strappendstr(cmd_path, path_table[i]);
		cmd_path = ft_strappendchar(cmd_path, '/');
		cmd_path = ft_strappendstr(cmd_path, cmd);
		if (stat(cmd_path, &info) == 0 && !S_ISDIR(info.st_mode))
			break ;
		i++;
		free(cmd_path);
		cmd_path = NULL;
	}
	ft_splitfree(path_table);
	return (cmd_path);
}

char	*get_cmd_path(char *cmd, t_list *env_list, int *status)
{
	const char	*path = env_list_get_value(env_list, "PATH");
	char		*cmd_path;

	cmd_path = NULL;
	if (ft_strchr(cmd, '/') || (!ft_strchr(cmd, '/') && path == NULL))
	{
		if (is_path_valid(cmd, status))
		{
			cmd_path = ft_strdup(cmd);
			if (!cmd_path)
				exit_error("ft_strdup() in get_cmd_path() malloc()");
		}
	}
	else
	{
		cmd_path = find_cmd_in_path(cmd, path);
		if (cmd_path == NULL)
		{
			*status = 127;
			error_msg(cmd, "command not found");
		}
	}
	return (cmd_path);
}
