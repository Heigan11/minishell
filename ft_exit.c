/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 18:42:15 by chilton           #+#    #+#             */
/*   Updated: 2021/05/18 21:35:43 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_str_numeric(char *str, int *status)
{
	long long	num;
	int			sign;

	if (*str == '-')
		sign = -1;
	else
		sign = 1;
	if (*str == '+' || *str =='-')
		str++;
	if (*str == '\0')
		return (0);
	num =0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		num *= 10;
		num += (*str - '0') * sign;
		if ((sign == 1 && num < 0) || (sign == -1 && num > 0))
			return (0);
		str++;
	}
	*status = num;
	return (1);
}

void	ft_exit(char **args, t_shell *sh)
{
	if (isatty(STDOUT_FILENO))
		printf("exit 🐚\n");
	if (args[0])
	{
		if (is_str_numeric(args[0], &sh->status))
		{
			if (args[1])
			{
				error_msg("exit", "too many arguments");
				sh->status = EXIT_FAILURE;
				return ;
			}
		}
		else
		{
			invalid_option_msg(args[0], "exit", "numeric argument required");
			sh->status = EXIT_OUT_OF_RANGE;
		}
	}
	save_history(&sh->history, sh->history_path);
	free_table(&sh->history, free);
	free(sh->history_path);
	ft_lstclear(&sh->env_list, env_free);
	free(sh->cmd_last_arg);
	exit((unsigned char)sh->status);
}
