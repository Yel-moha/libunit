/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_ft_strlen.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:10:15 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:26:00 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTS_FT_STRLEN_H
# define TESTS_FT_STRLEN_H

# include "../../framework/includes/libunit.h"
# include "../../libft/libft.h"

int	basic_test(void);
int	null_test(void);
int	segv_test(void);
int	bus_test(void);
int	sigabrt_test(void);
int	sigfpe_test(void);
int	sigpipe_test(void);
int	sigill_test(void);
int	one_char_test(void);
int	long_string_test(void);
int	spaces_test(void);
int	special_chars_test(void);
int	multibyte_test(void);
int	very_long_test(void);
int	alloc_test(void);
int	boundary_test(void);
int	compare_test(void);
int	overflow_test(void);
int	random_test(void);
int	null_pointer_test(void);
int	empty_string_test(void);
int	ft_strlen_launcher(void);

#endif
