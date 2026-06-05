/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_name.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moha <yel-moha@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 20:21:26 by yel-moha          #+#    #+#             */
/*   Updated: 2026/05/24 20:21:27 by yel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libunit.h"

static const struct s_sig_map
{
	int				signum;
	const char		*name;
}	g_sigtab[] = {
	{SIGSEGV, "SIGSEGV"},
	{SIGBUS, "SIGBUS"},
	{SIGABRT, "SIGABRT"},
	{SIGFPE, "SIGFPE"},
	{SIGPIPE, "SIGPIPE"},
	{SIGILL, "SIGILL"},
	{0, NULL}
};

const char	*signal_name(int sig)
{
	int	i;

	i = 0;
	while (g_sigtab[i].name)
	{
		if (g_sigtab[i].signum == sig)
			return (g_sigtab[i].name);
		i++;
	}
	return ("CRASH");
}
