/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 19:41:48 by chilton           #+#    #+#             */
/*   Updated: 2021/05/19 19:23:51 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_shlvl(t_list **env_list)
{
	char	*current_value;
	char	*key;
	char	*new_value;

	key = ft_strdup("SHLVL");
	current_value = env_list_get_value(*env_list, "SHLVL");
	if (current_value)
		new_value = ft_itoa(ft_atoi(current_value) + 1);
	else
		new_value = ft_strdup("1");
	if (!key || !new_value)
		exit_error("ft_strdup in set_shlvl() malloc()");
	env_list_update_key(env_list, key, new_value);
}

static void	init_pwd(t_list **env_list, char **history_path)
{
	char	*key;
	char	*value;
	char	pwd_path[PATH_MAX];

	key = ft_strdup("OLDPWD");
	if (!key)
		exit_error("ft_strdup() in init_pwd() malloc()");
	env_list_update_key(env_list, key, NULL);
	key = ft_strdup("PWD");
	if (!key)
		exit_error("ft_strdup in init_pwd() malloc()");
	value = NULL;
	if (getcwd(pwd_path, PATH_MAX))
	{
		value = ft_strdup(pwd_path);
		if (!value)
			exit_error("ft_strdup in init_pwd() malloc()");
		*history_path = ft_strjoin(pwd_path, "/");
		if (!history_path)
			exit_error("ft_strjoin() in init_pwd()");
		*history_path = ft_strappendstr(*history_path, HISTORY_FILE_NAME);
	}
	env_list_update_key(env_list, key, value);
}

static void	envp_to_list(char **envp, t_list **env_list, char **cmd_last_arg)
{
	size_t	i;
	char	**key_value;

	i = 0;
	while (envp[i])
	{
		key_value = ft_split(envp[i], '=');
		if (!key_value)
			exit_error("ft_split() in envp_to_list() malloc()");
		if (key_value[0] && ft_strcmp(key_value[0], "_") == 0)
		{
			set_last_arg(cmd_last_arg, key_value[1]);
			ft_splitfree(key_value);
			i++;
			continue ;
		}
		else if (ft_splitlen(key_value) == 1)
			env_list_add_back(env_list, key_value[0], ft_strdup(""));
		else
			env_list_add_back(env_list, key_value[0], key_value[1]);
		free(key_value);
		i++;
	}
}

static void	load_history(t_table *table, char *history_path)
{
	char		*line;
	int			fd;

	fd = open(history_path, O_RDONLY);
	if (fd == -1)
		error_msg(HISTORY_FILE_NAME, strerror(errno));
	line = NULL;
	while ((get_next_line(fd, &line) > 0))
		add_row_to_table(table, line);
	free(line);
	close(fd);
}

void	init_minishell(t_shell *sh, char **envp)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	envp_to_list(envp, &sh->env_list, &sh->cmd_last_arg);
	set_shlvl(&sh->env_list);
	init_pwd(&sh->env_list, &sh->history_path);
	load_history(&sh->history, sh->history_path);
}
