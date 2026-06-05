/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Launchers.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:13:21 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:13:22 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./framework/includes/libunit.h"

int	main(void)
{
	int	ret1;
	int	ret2;

	write(1, "\n", 1);
	write(1, "_\\|/__\\|/__\\|/__\\|/__\\|/__\\|/__\\|/__\\|/__\\|/_\n", 46);
	write(1, "*                                           *\n", 46);
	write(1, "*             LIBUNIT TEST SUITE            *\n", 46);
	write(1, "=============================================\n", 46);
	write(1, "\n", 1);
	ret1 = run_tests();
	write(1, "\n", 1);
	ret2 = run_real_tests();
	write(1, "\n", 1);
	if (ret1 == -1 || ret2 == -1)
		return (-1);
	return (0);
}
