/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_strlen.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:14:40 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:14:40 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTS_STRLEN_H
# define TESTS_STRLEN_H

# include "../../framework/includes/libunit.h"

int	basic_test(void);
int	null_test(void);
int	segv_test(void);
int	bus_test(void);

#endif
