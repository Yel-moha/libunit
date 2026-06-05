/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:08:01 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:21:57 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libunit.h"

void	load_test(t_unit_test **list, const char *name, int (*f)(void))
{
	t_unit_test	*new_node;
	t_unit_test	*temp;

	new_node = (t_unit_test *)malloc(sizeof(t_unit_test));
	if (!new_node)
		return ;
	new_node->name = ft_strdup(name);
	if (!new_node->name)
	{
		free(new_node);
		return ;
	}
	new_node->f = f;
	new_node->next = NULL;
	if (*list == NULL)
		*list = new_node;
	else
	{
		temp = *list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
}
