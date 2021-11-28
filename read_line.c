/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Jraye <jraye@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 19:20:46 by Jraye             #+#    #+#             */
/*   Updated: 2021/05/18 15:50:25 by Jraye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	arrow_up(char **line, t_table *history, size_t *ind)
{
	del_cycle(*line);
	if (history->size == 0)
		return ;
	if (*ind >= 1)
		--*ind;
	if (**line == '\0' && *ind < history->size - 1)
		++*ind;
	**line = '\0';
	ft_putstr_fd(history->table[*ind], STDOUT_FILENO);
	*line = ft_strappendstr(*line, history->table[*ind]);
}

static void	arrow_dawn(char **line, t_table *history, size_t *ind)
{
	del_cycle(*line);
	if (history->size == 0)
		return ;
	if (*ind < history->size - 1)
	{
		++*ind;
		**line = '\0';
		ft_putstr_fd(history->table[*ind], STDOUT_FILENO);
		*line = ft_strappendstr(*line, history->table[*ind]);
	}
	else if (*ind == history->size - 1)
		**line = '\0';
}

static struct termios	term_init(void)
{
	char			*term;
	struct termios	old_term;
	struct termios	new_term;

	if (isatty(STDIN_FILENO) == 0)
	{
		ft_putendl_fd("STDIN is not terminal!", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (tcgetattr(STDIN_FILENO, &old_term) != 0)
		exit_error("tcgetattr() in term_init()");
	new_term = old_term;
	new_term.c_lflag &= ~(ECHO | ICANON | ISIG);
	new_term.c_cc[VMIN] = 0;
	new_term.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term) != 0)
		exit_error("tcsetattr() in term_init()");
	term = getenv("TERM");
	if (term == NULL)
		term = "xterm-256color";
	if (tgetent(STDIN_FILENO, term) != 1)
		exit_error("tgetent() in term_init()");
	tputs(keypad_xmit, 1, ft_putchar);
	ft_putstr_fd(PROMPTN, STDOUT_FILENO);
	return (old_term);
}

static void	read_cycle(char **line, size_t histoty_ind,
		t_shell *sh, struct termios *old_term)
{
	char	buf[22];
	int		readed;

	buf[0] = '\0';
	while (buf[0] != '\n')
	{
		readed = read(STDIN_FILENO, buf, 21);
		buf[readed] = '\0';
		if (ft_strcmp(buf, key_up) == 0)
			arrow_up(line, &sh->history, &histoty_ind);
		else if (ft_strcmp(buf, key_down) == 0)
			arrow_dawn(line, &sh->history, &histoty_ind);
		else if (ft_strcmp(buf, BACKSPACE) == 0)
			del_one(*line);
		else if (**line == '\0' && ft_strcmp(buf, CTRL_D) == 0)
			quit(*line, old_term, sh);
		else if (ft_strcmp(buf, CTRL_C) == 0)
		{
			**line = '\0';
			break ;
		}
		else if (buf[0] >= ' ' && buf[0] <= '~')
			add_symbol(line, buf);
	}
}

char	*read_line(t_shell *shell)
{
	struct termios	old_term;
	char			*line;

	old_term = term_init();
	line = ft_strdup("");
	if (!line)
		exit_error("ft_strdup() in read_line() malloc()");
	read_cycle(&line, shell->history.size, shell, &old_term);
	if (tcsetattr(STDOUT_FILENO, TCSANOW, &old_term))
		exit_error("tcsetattr() in read_line()");
	add_to_history(line, &shell->history);
	write(STDOUT_FILENO, "\n", 1);
	return (line);
}
