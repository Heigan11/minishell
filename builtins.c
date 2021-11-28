/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 16:23:49 by Jraye             #+#    #+#             */
/*   Updated: 2021/05/19 18:12:54 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(char **args, t_list *env_list, char *last_arg, int *status)
{
	t_env_item	*env_item;

	*status = EXIT_SUCCESS;
	if (args[0])
	{
		error_msg("env", "options and arguments not supported! ✅");
		*status = EXIT_FAILURE;
		return ;
	}
	while (env_list)
	{
		env_item = env_list->content;
		if (env_item->value)
			printf("%s=%s\n", env_item->key, env_item->value);
		env_list = env_list->next;
	}
	if (last_arg)
		printf("_=%s\n", last_arg);
}

void	ft_echo(char **args, int *status)
{
	int		new_line;
	ssize_t	n;

	*status = EXIT_SUCCESS;
	new_line = 1;
	if (args[0] && ft_strcmp(args[0], "-n") == 0)
	{
		new_line = 0;
		args++;
	}
	while (*args)
	{
		n = write(STDOUT_FILENO, *args, ft_strlen(*args));
		if (n < 0)
			*status = EXIT_FAILURE;
		if (*(args + 1))
			n = write(STDOUT_FILENO, " ", 1);
		if (n < 0)
			*status = EXIT_FAILURE;
		args++;
	}
	if (new_line)
		n = write(STDOUT_FILENO, "\n", 1);
	if (n < 0)
		*status = EXIT_FAILURE;
}

void	ft_unset(char **args, t_list **env_list, int *status)
{
	size_t	i;

	i = 0;
	*status = EXIT_SUCCESS;
	while (args[i])
	{
		if (is_id_valid(args[i]))
			ft_lstremoveif(env_list, args[i], env_cmp, env_free);
		else
		{
			invalid_id_msg(args[i], "unset");
			*status = EXIT_FAILURE;
		}
		i++;
	}
}

void	ft_pwd(int *status)
{
	char	path[PATH_MAX];

	*status = EXIT_SUCCESS;
	if (getcwd(path, PATH_MAX))
		printf("%s\n", path);
	else
	{
		error_msg("pwd", strerror(errno));
		*status = errno;
	}
}
