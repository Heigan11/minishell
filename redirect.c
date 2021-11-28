/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 20:50:52 by chilton           #+#    #+#             */
/*   Updated: 2021/05/19 21:03:49 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	redirect_error(char *file_name, char *msg, int *status)
{
	*status = EXIT_FAILURE;
	error_msg(file_name, msg);
}

static int	get_flags(t_redir_type type)
{
	int	flags;

	if (type == OUT)
		flags = O_TRUNC | O_WRONLY | O_CREAT;
	else if (type == OUT_APPEND)
		flags = O_APPEND | O_WRONLY | O_CREAT;
	else
		flags = O_RDONLY;
	return (flags);
}

static void	set_redirect(int fd, t_redir_type type)
{
	int	d;

	if (type == OUT || type == OUT_APPEND)
		d = dup2(fd, STDOUT_FILENO);
	else
		d = dup2(fd, STDIN_FILENO);
	if (d == -1)
		exit_error("dup2() in make_redirect()");
	close(fd);
}

int	make_redirects(t_redirect **redirs, t_shell *sh)
{
	int		fd;
	char	*f_name;

	while (redirs && *redirs)
	{
		f_name = parse_token((*redirs)->file_name, sh);
		if (f_name == NULL)
		{
			redirect_error((*redirs)->file_name,
				"ambiguous redirect", &sh->status);
			return (0);
		}
		fd = open(f_name, get_flags((*redirs)->type),
				  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fd == -1)
		{
			redirect_error(f_name, strerror(errno), &sh->status);
			free(f_name);
			return (0);
		}
		set_redirect(fd, (*redirs)->type);
		free(f_name);
		redirs++;
	}
	return (1);
}
