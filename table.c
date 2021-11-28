/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 18:31:31 by chilton           #+#    #+#             */
/*   Updated: 2021/05/19 19:25:31 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_row_to_table(t_table *table, void *new_row)
{
	void	**ptr;

	if (new_row)
	{
		if (table->available == 0)
		{
			ptr = malloc(sizeof(void *) * (table->size + TABLE_BLOCK_SIZE + 1));
			if (!ptr)
				exit_error("add_row_to_table() malloc()");
			table->available = TABLE_BLOCK_SIZE;
			ft_memcpy(ptr, table->table, sizeof(void *) * table->size);
			free(table->table);
			table->table = ptr;
		}
		table->table[table->size] = new_row;
		table->size++;
		table->available--;
		table->table[table->size] = NULL;
	}
}

void	free_table(t_table *table, void (*del)(void*))
{
	size_t	i;

	if (table->table)
	{
		i = 0;
		while (table->table[i])
		{
			del(table->table[i]);
			i++;
		}
	}
	free(table->table);
}

char	**env_list_to_table(t_list *list, char *cmd_last_arg)
{
	char		*line;
	t_table		table;
	t_env_item	*e_item;

	table = (t_table){NULL, 0, 0};
	while (list)
	{
		e_item = list->content;
		if (e_item->value)
		{
			line = ft_strjoin(e_item->key, "=");
			if (!line)
				exit_error("ft_strjoin() in list_to_table() malloc()");
			line = ft_strappendstr(line, e_item->value);
			add_row_to_table(&table, line);
		}
		list = list->next;
	}
	line = ft_strdup("_=");
	if (!line)
		exit_error("ft_strdup() in env_list_to_table() malloc()");
	line = ft_strappendstr(line, cmd_last_arg);
	add_row_to_table(&table, line);
	return ((char **)table.table);
}

void	sort_table(t_table *table, int (*cmp)())
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	while (i < table->size - 1)
	{
		j = 0;
		while (j < table->size - i - 1)
		{
			if (cmp(table->table[j], table->table[j + 1]) > 0)
			{
				tmp = table->table[j];
				table->table[j] = table->table[j + 1];
				table->table[j +1 ] = tmp;
			}
			j++;
		}
		i++;
	}
}
