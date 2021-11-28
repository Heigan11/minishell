/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 17:44:24 by chilton           #+#    #+#             */
/*   Updated: 2021/05/18 21:34:05 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	single_quotes(char **token, char **parsed)
{
	char	*close_quote;

	++*token;
	close_quote = ft_strchr(*token, '\'');
	*close_quote = '\0';
	*parsed = ft_strappendstr(*parsed, *token);
	*token = close_quote + 1;
}

static char	*get_from_env(char **token, t_list *env_list)
{
	const char	*start = *token;
	char		*key;
	char		*value;

	while (**token && (ft_isalnum(**token) || **token == '_'))
		++*token;
	key = ft_substr(start, 0, *token - start);
	if (!start)
		exit_error("ft_substr() in get_env_value() malloc()");
	value = env_list_get_value(env_list, key);
	free(key);
	return (value);
}

static void	dollar(char **token, char **parsed, t_shell *sh)
{
	char	*tmp;
	char	*value;

	++*token;
	tmp = NULL;
	if (**token == '?')
	{
		++*token;
		tmp = ft_itoa(sh->status);
		if (!tmp)
			exit_error("ft_itoa() in dollar() malloc()");
		value = tmp;
	}
	else if (**token == '_' && !ft_isalnum((*token)[1]) && (*token)[1] != '_')
	{
		++*token;
		value = sh->cmd_last_arg;
	}
	else if (!ft_isalpha(**token) && **token != '_')
		value = "$";
	else
		value = get_from_env(token, sh->env_list);
	*parsed = ft_strappendstr(*parsed, value);
	free(tmp);
}

static void	double_quotes(char **token, char **parsed, t_shell *sh)
{
	++*token;
	*parsed = ft_strappendstr(*parsed, "");
	while (**token != '\"')
	{
		if (**token == '\\' && ft_strchr("\"$`\\", *(*token + 1)))
		{
			*parsed = ft_strappendchar(*parsed, *(*token + 1));
			*token = *token + 2;
		}
		else if (**token == '$')
			dollar(token, parsed, sh);
		else
		{
			*parsed = ft_strappendchar(*parsed, **token);
			++*token;
		}
	}
	++*token;
}

char	*parse_token(char *token, t_shell *sh)
{
	char	*parsed;

	parsed = NULL;
	while (*token)
	{
		if (*token == '\\')
		{
			parsed = ft_strappendchar(parsed, *(token + 1));
			token += 2;
		}
		else if (*token == '\'')
			single_quotes(&token, &parsed);
		else if (*token == '\"')
			double_quotes(&token, &parsed, sh);
		else if (*token == '$' && (token[1] == '\'' || token[1] == '\"'))
			token++;
		else if (*token == '$')
			dollar(&token, &parsed, sh);
		else
		{
			parsed = ft_strappendchar(parsed, *token);
			token++;
		}
	}
	return (parsed);
}
