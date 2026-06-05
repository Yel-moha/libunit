/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   13_compare.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:08:01 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:25:06 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests_ft_strlen.h"

int	compare_test(void)
{
	const char	*s;

	s = "confronto con strlen";
	if (ft_strlen(s) == strlen(s))
		return (0);
	return (-1);
}
