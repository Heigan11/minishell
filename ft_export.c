/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 16:41:09 by Jraye             #+#    #+#             */
/*   Updated: 2021/05/19 16:41:35 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_export(t_list *e_lst)
{
	t_table		export_table;
	char		*row;
	size_t		i;

	export_table = (t_table){NULL, 0, 0};
	while (e_lst)
	{
		row = ft_strjoin("declare -x ", ((t_env_item *)e_lst->content)->key);
		if (!row)
			exit_error("ft_strjont() in print_export() malloc()");
		if (((t_env_item *)e_lst->content)->value)
		{
			row = ft_strappendstr(row, "=\"");
			row = ft_strappendstr(row, ((t_env_item *)e_lst->content)->value);
			row = ft_strappendchar(row, '\"');
		}
		add_row_to_table(&export_table, row);
		e_lst = e_lst->next;
	}
	sort_table(&export_table, ft_strcmp);
	i = 0;
	while (export_table.table[i])
		printf("%s\n", export_table.table[i++]);
	free_table(&export_table, free);
}

static void	set_env_variable(char *key_value, t_list **env_list, int *status)
{
	char	*ptr;

	ptr = ft_strchr(key_value, '=');
	if (ptr)
		*ptr = '\0';
	if (ft_strcmp(key_value, "_") == 0)
		return ;
	if (!is_id_valid(key_value))
	{
		invalid_id_msg(key_value, "export");
		*status = EXIT_FAILURE;
	}
	else if (ptr)
		env_list_update_key(env_list,
			ft_strdup(key_value), ft_strdup(ptr + 1));
	else if (!ft_lstgetitem(*env_list, key_value, env_cmp))
		env_list_update_key(env_list, ft_strdup(key_value), NULL);
}

void	ft_export(char **args, t_list **env_list, int *status)
{
	*status = EXIT_SUCCESS;
	if (args[0] == NULL)
	{
		print_export(*env_list);
		return ;
	}
	while (*args)
	{
		set_env_variable(*args, env_list, status);
		args++;
	}
}
