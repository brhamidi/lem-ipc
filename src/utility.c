/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 15:55:51 by bhamidi           #+#    #+#             */
/*   Updated: 2019/01/11 15:57:57 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

extern t_proc g_clean;

int		get_nplayer(const char *str)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (i < MAP_SIZE * MAP_SIZE)
	{
		if (str[i] != -1)
			++result;
		++i;
	}
	return (result);
}

void	send_finish(int msqid, const char *str)
{
	int				n;
	struct s_msgbuf	buf;
	int				timeout;

	n = get_nplayer(str);
	buf.mtype = 4242;
	timeout = 1000;
	while (n && timeout)
	{
		if (msgsnd(msqid, (void *)&buf, sizeof(buf.mtext), IPC_NOWAIT) != -1)
			--n;
		usleep(1000);
		--timeout;
	}
}

int		nteam(const char *str)
{
	int	i;
	int	nteam;
	int	buf;

	i = 0;
	buf = 0;
	nteam = 0;
	while (i < MAP_SIZE * MAP_SIZE)
	{
		if (str[i] != -1)
		{
			if (buf == 0)
			{
				buf = str[i];
				++nteam;
			}
			if (buf != str[i])
				return (2);
		}
		++i;
	}
	return (nteam);
}

int		retry(void)
{
	int	c;

	while (1)
	{
		c = getch();
		if (c == 'y' || c == 'n')
		{
			clear();
			return (c == 'y' ? 1 : 0);
		}
		usleep(TIME);
	}
	return (0);
}
