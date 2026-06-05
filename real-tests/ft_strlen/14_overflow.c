/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   14_overflow.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:08:01 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:25:10 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests_ft_strlen.h"

int	overflow_test(void)
{
	size_t	huge;
	char	*s;
	int		ret;

	huge = 1000000;
	s = malloc(huge + 1);
	if (!s)
		return (-1);
	memset(s, 'a', huge);
	s[huge] = '\0';
	if (ft_strlen(s) == huge)
		ret = 0;
	else
		ret = -1;
	free(s);
	return (ret);
}
