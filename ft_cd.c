/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 17:25:52 by Jraye             #+#    #+#             */
/*   Updated: 2021/05/19 17:53:34 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_new_path(t_list *env_list, char *key, char *msg, int *status)
{
	char	*new_path;

	new_path = env_list_get_value(env_list, key);
	if (!new_path)
	{
		error_msg("cd", msg);
		*status = EXIT_FAILURE;
	}
	return (new_path);
}

static void	change_dir(char *new_pwd, t_list *env_list, int *status)
{
	char	tmp_path[PATH_MAX];

	getcwd(tmp_path, PATH_MAX);
	if (chdir(new_pwd) == -1)
	{
		invalid_option_msg(new_pwd, "cd", strerror(errno));
		*status = EXIT_FAILURE;
		return ;
	}
	if (ft_lstgetitem(env_list, "OLDPWD", env_cmp))
		env_list_update_key(&env_list,
			ft_strdup("OLDPWD"), ft_strdup(tmp_path));
	if (!getcwd(tmp_path, PATH_MAX))
		error_msg("cd", strerror(errno));
	if (ft_lstgetitem(env_list, "PWD", env_cmp))
		env_list_update_key(&env_list, ft_strdup("PWD"), ft_strdup(tmp_path));
}

void	ft_cd(char **args, t_list *env_list, int *status)
{
	char	*new_wd;

	*status = EXIT_SUCCESS;
	if (args[0] == NULL)
		new_wd = get_new_path(env_list, "HOME", "HOME not set", status);
	else if (!ft_strcmp(args[0], "-"))
		new_wd = get_new_path(env_list, "OLDPWD", "OLDPWD not set", status);
	else
		new_wd = *args;
	if (!new_wd)
		return ;
	change_dir(new_wd, env_list, status);
	if (*status == EXIT_SUCCESS && args[0] && !ft_strcmp(args[0], "-"))
		ft_pwd(status);
}
