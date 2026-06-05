/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_tests.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:08:01 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:22:02 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libunit.h"

int	launch_tests(t_unit_test **list, const char *func_name)
{
	t_unit_test		*curr;
	int				tot;
	int				ok;

	tot = 0;
	ok = 0;
	curr = *list;
	while (curr)
	{
		if (run_one_test(curr, func_name, &tot, &ok) == -1)
		{
			free_list(list);
			return (-1);
		}
		curr = curr->next;
	}
	print_summary(ok, tot);
	free_list(list);
	if (ok == tot)
		return (0);
	return (-1);
}
