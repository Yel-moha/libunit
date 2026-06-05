/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   15_random_test.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:08:01 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:25:16 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests_ft_strlen.h"

int	random_test(void)
{
	const char	*s;

	s = "42 Network!";
	if (ft_strlen(s) == 11)
		return (0);
	return (-1);
}
