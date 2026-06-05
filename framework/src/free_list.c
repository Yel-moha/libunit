/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:08:01 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:22:12 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libunit.h"

void	free_list(t_unit_test **list)
{
	t_unit_test	*curr;
	t_unit_test	*next;

	curr = *list;
	while (curr)
	{
		next = curr->next;
		free(curr->name);
		free(curr);
		curr = next;
	}
	*list = NULL;
}
