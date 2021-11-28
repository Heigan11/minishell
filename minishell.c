/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 15:03:35 by Jraye             #+#    #+#             */
/*   Updated: 2021/05/19 18:11:27 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute(char **tokens, t_shell *sh)
{
	t_table		commands;

	while (*tokens)
	{
		commands = (t_table){NULL, 0, 0};
		get_commands(&tokens, &commands, sh);
		if (commands.size == 1)
			exec_simple_cmd(commands.table[0], sh);
		else
			exec_pipe_cmds((t_command **)commands.table, commands.size, sh);
		free_table(&commands, clear_command);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**tokens;
	t_shell	sh;

	(void)argc;
	(void)argv;
	sh = (t_shell){NULL, (t_table){NULL, 0, 0}, NULL, NULL, 0};
	init_minishell(&sh, envp);
	while (1)
	{
		line = read_line(&sh);
		tokens = split_line_to_tokens(line, &sh.status);
		if (tokens)
			execute(tokens, &sh);
		ft_splitfree(tokens);
		free(line);
	}
}
