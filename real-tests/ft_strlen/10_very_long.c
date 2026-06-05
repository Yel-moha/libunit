/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10_very_long.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:08:01 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:24:52 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests_ft_strlen.h"

int	very_long_test(void)
{
	char	*s;
	int		ret;

	s = malloc(10001);
	if (!s)
		return (-1);
	memset(s, 'x', 10000);
	s[10000] = '\0';
	if (ft_strlen(s) == 10000)
		ret = 0;
	else
		ret = -1;
	free(s);
	return (ret);
}
