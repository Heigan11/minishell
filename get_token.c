/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 17:23:04 by Jraye             #+#    #+#             */
/*   Updated: 2021/05/10 19:27:57 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	multiline_error(char **line, int *status)
{
	*status = SYNTAX_ERROR;
	*line = NULL;
	ft_putendl_fd("minshell: multiline commands not supported! ✅",
		STDERR_FILENO);
}

static void	single_quotes(char **line, int *status)
{
	char	*close_quote;

	++*line;
	close_quote = ft_strchr(*line, '\'');
	if (close_quote)
		*line = close_quote + 1;
	else
		multiline_error(line, status);
}

static void	double_quotes(char **line, int *status)
{
	++*line;
	while (*line && **line && **line != '\"')
	{
		if (**line == '\\' && ft_strchr("\"$\\", *(*line + 1)))
			*line = *line + 2;
		else
			++*line;
	}
	if (**line == '\0')
		multiline_error(line, status);
	else
		++*line;
}

char	*get_token(char **line, int *status)
{
	const char	*token_start = *line;
	char		*new_token;

	if (!ft_strncmp(*line, ">>", 2))
		*line = *line + 2;
	else if (**line == '|' || **line == ';' || **line == '<' || **line == '>')
		++*line;
	else
		while (*line && **line && !ft_strchr(" ;|<>", **line))
			if (**line == '\\' && *(*line + 1) == '\0')
				multiline_error(line, status);
	else if (**line == '\\')
		*line = *line + 2;
	else if (**line == '\'')
		single_quotes(line, status);
	else if (**line == '\"')
		double_quotes(line, status);
	else
		++*line;
	if (*line == NULL || token_start == *line)
		return (NULL);
	new_token = ft_substr(token_start, 0, *line - token_start);
	if (!new_token)
		exit_error("ft_substr in get_token() malloc()");
	return (new_token);
}
