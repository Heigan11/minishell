/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 20:24:08 by chilton           #+#    #+#             */
/*   Updated: 2021/05/18 21:35:18 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redirect	*get_redirect(char **token)
{
	t_redirect	*redirect;

	redirect = malloc(sizeof(t_redirect));
	if (!redirect)
		exit_error("get_redirect() malloc()");
	if (!ft_strcmp(*token, ">>"))
		redirect->type = OUT_APPEND;
	else if (!ft_strcmp(*token, ">"))
		redirect->type = OUT;
	else
		redirect->type = IN;
	redirect->file_name = *(token + 1);
	return (redirect);
}

static t_command	*get_command(char ***tokens, t_shell *sh)
{
	t_command	*cmd;
	t_table		args;
	t_table		redirects;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		exit_error("get_command() malloc()");
	args = (t_table){NULL, 0, 0};
	redirects = (t_table){NULL, 0, 0};
	while (**tokens && ft_strcmp(";", **tokens) && ft_strcmp("|", **tokens))
	{
		if (***tokens == '>' || ***tokens == '<')
		{
			add_row_to_table(&redirects, get_redirect(*tokens));
			*tokens += 2;
		}
		else
		{
			add_row_to_table(&args, parse_token(**tokens, sh));
			++*tokens;
		}
	}
	cmd->args = (char **)args.table;
	cmd->redirs = (t_redirect **)redirects.table;
	return (cmd);
}

void	get_commands(char ***tokens, t_table *commands, t_shell *sh)
{
	t_command	*cmd;

	while (**tokens && ft_strcmp(**tokens, ";"))
	{
		cmd = get_command(tokens, sh);
		add_row_to_table(commands, cmd);
		if (**tokens && !ft_strcmp(**tokens, "|"))
			++*tokens;
	}
	if (**tokens && !ft_strcmp(**tokens, ";"))
		++*tokens;
}
