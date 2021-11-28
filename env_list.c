/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Jraye <jraye@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/06 16:43:51 by chilton           #+#    #+#             */
/*   Updated: 2021/05/19 19:10:02 by Jraye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_list_add_back(t_list **lst, char *new_key, char *new_value)
{
	t_env_item	*e_item;
	t_list		*new;

	new = malloc(sizeof(t_list));
	e_item = malloc(sizeof(t_env_item));
	if (!new || !e_item)
		exit_error("env_list_add_back() malloc()");
	e_item->key = new_key;
	e_item->value = new_value;
	new->content = e_item;
	new->next = NULL;
	ft_lstadd_back(lst, new);
}

void	env_list_update_key(t_list **lst, char *key, char *new_value)
{
	t_env_item	*e_item;

	e_item = ft_lstgetitem(*lst, key, env_cmp);
	if (e_item)
	{
		free(e_item->key);
		free(e_item->value);
		e_item->key = key;
		e_item->value = new_value;
	}
	else
		env_list_add_back(lst, key, new_value);
}

char	*env_list_get_value(t_list *lst, char *key)
{
	t_env_item	*e_item;

	e_item = ft_lstgetitem(lst, key, env_cmp);
	if (e_item)
		return (e_item->value);
	return (NULL);
}

void	env_free(void *env_item)
{
	t_env_item	*ptr;

	ptr = env_item;
	free(ptr->key);
	free(ptr->value);
	free(ptr);
}

int	env_cmp(t_env_item *env, const char *key)
{
	return (ft_strcmp(env->key, key));
}
