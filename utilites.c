/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilites.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 16:14:23 by chilton           #+#    #+#             */
/*   Updated: 2021/05/17 17:23:52 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	save_history(t_table *history, char *history_path)
{
	int			fd;
	size_t		i;

	if (history->size == 0)
		return ;
	fd = open(history_path, O_WRONLY | O_CREAT | O_TRUNC,
			S_IWUSR | S_IRUSR);
	if (fd == -1)
	{
		error_msg(history_path, strerror(errno));
		return ;
	}
	i = 0;
	while (i < history->size)
	{
		write(fd, history->table[i], ft_strlen(history->table[i]));
		write(fd, "\n", 1);
		i++;
	}
	close(fd);
}

char	*ft_strappendstr(char *line, char *added_line)
{
	char	*new_str;

	if (line == NULL && added_line == NULL)
		return (NULL);
	else if (line == NULL)
		new_str = ft_strdup(added_line);
	else if (added_line == NULL)
		return (line);
	else
		new_str = ft_strjoin(line, added_line);
	if (!new_str)
		exit_error("ft_strappendstr() malloc()");
	free(line);
	return (new_str);
}

char	*ft_strappendchar(char *line, char c)
{
	char	add[2];

	add[0] = c;
	add[1] = '\0';
	return (ft_strappendstr(line, add));
}

int	is_id_valid(char *identifer)
{
	if (!ft_isalpha(*identifer) && *identifer != '_')
		return (0);
	identifer++;
	while (*identifer)
	{
		if (!ft_isalnum(*identifer) && *identifer != '_')
			return (0);
		identifer++;
	}
	return (1);
}

void	clear_command(void *ptr)
{
	size_t		i;
	t_command	*cmd;

	cmd = ptr;
	ft_splitfree(cmd->args);
	if (cmd->redirs)
	{
		i = 0;
		while (cmd->redirs[i])
		{
			free(cmd->redirs[i]);
			i++;
		}
		free(cmd->redirs);
	}
	free(cmd);
}
