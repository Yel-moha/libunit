/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_summary.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:08:01 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:21:41 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libunit.h"

void	print_summary(int ok, int tot)
{
	int	len_ok;
	int	len_tot;
	int	total_len;
	int	pad;

	len_ok = num_len(ok);
	len_tot = num_len(tot);
	total_len = len_ok + 1 + len_tot + 14;
	write(1, "\n---------------------------------------------\n", 46);
	write(1, "\n", 1);
	pad = (45 - total_len) / 2;
	if (pad < 0)
		pad = 0;
	while (pad-- > 0)
		write(1, " ", 1);
	ft_putnbr_fd(ok, 1);
	write(1, "/", 1);
	ft_putnbr_fd(tot, 1);
	write(1, " tests checked", 14);
	write(1, "\n", 1);
}
