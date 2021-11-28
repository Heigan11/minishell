/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 12:09:19 by Jraye             #+#    #+#             */
/*   Updated: 2021/05/17 21:29:27 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del_cycle(char *line)
{
	int	i;

	i = ft_strlen(line);
	while (i > 0)
	{
		tputs(delete_character, 1, ft_putchar);
		tputs(cursor_left, 1, ft_putchar);
		tputs(delete_character, 1, ft_putchar);
		i--;
	}
}

void	del_one(char *line)
{
	int	i;

	i = ft_strlen(line);
	if (i > 0)
	{
		tputs(delete_character, 1, ft_putchar);
		tputs(cursor_left, 1, ft_putchar);
		tputs(delete_character, 1, ft_putchar);
		line[i - 1] = '\0';
	}
}

void	add_symbol(char **line, char *buf)
{
	ft_putstr_fd(buf, STDOUT_FILENO);
	*line = ft_strappendstr(*line, buf);
}

void	quit(char *line, struct termios *old_term, t_shell *sh)
{
	char	*args[1];

	args[0] = NULL;
	del_cycle(line);
	free(line);
	if (tcsetattr(STDIN_FILENO, TCSANOW, old_term) != 0)
		exit_error("tcsetattr() in quit()");
	ft_exit(args, sh);
}

void	add_to_history(char *line, t_table *history)
{
	char	*str;

	if (ft_strlen(line) > 0)
	{
		str = ft_strdup(line);
		if (!str)
			exit_error("ft_strdup() in add_to_history() malloc()");
		add_row_to_table(history, str);
	}
}
