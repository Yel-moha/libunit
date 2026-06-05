/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_result.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:08:01 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:21:45 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libunit.h"

void	print_result(const char *func, const char *test,
		const char *status, const char *color)
{
	write(1, "[", 1);
	write(1, func, ft_strlen(func));
	write(1, "]:[", 3);
	write(1, test, ft_strlen(test));
	write(1, "]:[", 3);
	putstr_color(status, color, 1);
	write(1, "]\n", 2);
}
