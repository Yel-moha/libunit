/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putstr_color.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:08:01 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:21:37 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libunit.h"

void	putstr_color(const char *str, const char *color, int fd)
{
	if (color)
	{
		write(fd, color, ft_strlen(color));
		write(fd, str, ft_strlen(str));
		write(fd, ANSI_RESET, ft_strlen(ANSI_RESET));
	}
	else
	{
		write(fd, str, ft_strlen(str));
	}
}
