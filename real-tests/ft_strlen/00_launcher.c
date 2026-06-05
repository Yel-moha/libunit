/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_launcher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:08:01 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:23:53 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests_ft_strlen.h"

int	ft_strlen_launcher(void)
{
	t_unit_test	*testlist;

	testlist = NULL;
	load_test(&testlist, "Basic test", basic_test);
	load_test(&testlist, "NULL test", null_test);
	load_test(&testlist, "SEGV test", segv_test);
	load_test(&testlist, "BUS test", bus_test);
	load_test(&testlist, "One char", one_char_test);
	load_test(&testlist, "Long string", long_string_test);
	load_test(&testlist, "Spaces", spaces_test);
	load_test(&testlist, "Special chars", special_chars_test);
	load_test(&testlist, "Multibyte", multibyte_test);
	load_test(&testlist, "Very long", very_long_test);
	load_test(&testlist, "Alloc test", alloc_test);
	load_test(&testlist, "Boundary", boundary_test);
	load_test(&testlist, "Compare", compare_test);
	load_test(&testlist, "Overflow", overflow_test);
	load_test(&testlist, "Random", random_test);
	load_test(&testlist, "Null pointer", null_pointer_test);
	load_test(&testlist, "Empty string", empty_string_test);
	load_test(&testlist, "SIGABRT test", sigabrt_test);
	load_test(&testlist, "SIGFPE test", sigfpe_test);
	load_test(&testlist, "SIGPIPE test", sigpipe_test);
	load_test(&testlist, "SIGILL test", sigill_test);
	return (launch_tests(&testlist, "FT_STRLEN"));
}
