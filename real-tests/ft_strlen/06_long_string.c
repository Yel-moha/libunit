/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_long_string.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:24:23 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:24:27 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests_ft_strlen.h"

int	long_string_test(void)
{
	if (ft_strlen("questa è una stringa abbastanza lunga per testare") == 49)
		return (0);
	return (-1);
}
