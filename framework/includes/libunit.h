/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libunit.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:10:45 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:22:25 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBUNIT_H
# define LIBUNIT_H

# define ANSI_GREEN "\033[32m"
# define ANSI_RED "\033[31m"
# define ANSI_YELLOW "\033[33m"
# define ANSI_RESET "\033[0m"

# include "../../libft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <signal.h>

typedef struct s_unit_test
{
	char				*name;
	int					(*f)(void);
	struct s_unit_test	*next;
}	t_unit_test;

void		load_test(t_unit_test **list, const char *name, int (*f)(void));
int			launch_tests(t_unit_test **list, const char *func_name);
int			run_tests(void);
int			run_real_tests(void);
int			strlen_launcher(void);
int			ft_strlen_launcher(void);
void		free_list(t_unit_test **list);
void		putstr_color(const char *str, const char *color, int fd);
int			num_len(int n);
void		print_result(const char *func, const char *test,
				const char *status, const char *color);
void		print_summary(int ok, int tot);
const char	*signal_name(int sig);
void		handle_result(int status, const char *func_name,
				const char *test_name, int *ok);
int			run_one_test(t_unit_test *test, const char *func_name,
				int *tot, int *ok);

#endif
