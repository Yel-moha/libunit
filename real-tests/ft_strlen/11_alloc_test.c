/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   11_alloc_test.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:08:01 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:24:57 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests_ft_strlen.h"

int	alloc_test(void)
{
	char	*s;
	int		ret;

	s = strdup("alloc test");
	if (!s)
		return (-1);
	if (ft_strlen(s) == 10)
		ret = 0;
	else
		ret = -1;
	free(s);
	return (ret);
}
