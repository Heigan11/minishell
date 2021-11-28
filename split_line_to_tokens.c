/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_line_to_tokens.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/29 16:56:42 by chilton           #+#    #+#             */
/*   Updated: 2021/05/17 16:54:05 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	syntax_error_detected(char **tokens, size_t size, int *status)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		if ((tokens[i][0] == '|'
			&& (i == 0 || i == size - 1 \
			|| ft_strchr("><;|", tokens[i - 1][0])
			|| ft_strchr(";|", tokens[i + 1][0])))
			||
			(tokens[i][0] == ';' &&
			(i == 0 || ft_strchr("><;|", tokens[i - 1][0])))
			||
			(ft_strchr("><", tokens[i][0]) && \
			(i == size - 1 || ft_strchr("><;|", tokens[i + 1][0]))))
		{
			*status = SYNTAX_ERROR;
			ft_putendl_fd("minshell: syntax error", STDERR_FILENO);
			return (1);
		}
		i++;
	}
	return (0);
}

char	**split_line_to_tokens(char *line, int *status)
{
	char	*token;
	t_table	tokens;

	tokens = (t_table){NULL, 0, 0};
	while (line && *line)
	{
		while (*line == ' ')
			line++;
		token = get_token(&line, status);
		add_row_to_table(&tokens, token);
	}
	if (line == NULL || tokens.size == 0
		|| syntax_error_detected((char **)tokens.table, tokens.size, status))
	{
		free_table(&tokens, free);
		return (NULL);
	}
	return ((char **)tokens.table);
}
