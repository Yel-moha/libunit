/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_result.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:08:01 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:22:06 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libunit.h"

void	handle_result(int status, const char *func_name,
		const char *test_name, int *ok)
{
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
		{
			print_result(func_name, test_name, "OK", ANSI_GREEN);
			(*ok)++;
		}
		else
			print_result(func_name, test_name, "KO", ANSI_RED);
	}
	else if (WIFSIGNALED(status))
		print_result(func_name, test_name,
			signal_name(WTERMSIG(status)), ANSI_YELLOW);
}
