/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_one_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:08:01 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:21:32 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libunit.h"

int	run_one_test(t_unit_test *test, const char *func_name,
		int *tot, int *ok)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (write(2, "fork error\n", 11) * 0 - 1);
	if (pid == 0)
		exit(test->f());
	if (wait(&status) == -1)
		return (write(2, "wait error\n", 11) * 0 - 1);
	(*tot)++;
	handle_result(status, func_name, test->name, ok);
	return (0);
}
