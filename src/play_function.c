/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play_function.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 16:00:40 by bhamidi           #+#    #+#             */
/*   Updated: 2019/01/11 16:04:32 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

extern int	(*g_tab[4])(t_proc *);

int		get_ally(const char *str, int number)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (i < MAP_SIZE * MAP_SIZE)
	{
		if (str[i] == number)
			++result;
		++i;
	}
	return (result - 1);
}

int		send_position(t_proc *e, int *opp, struct s_msgbuf *buf)
{
	int	n_ally;

	if ((n_ally = get_ally((const char *)e->ptr, e->number)) == 0)
		return (0);
	while (n_ally)
	{
		buf->mtype = e->number;
		buf->mtext[0] = opp[0];
		buf->mtext[4] = opp[1];
		msgsnd(e->msqid, (void *)buf, 8, IPC_NOWAIT);
		--n_ally;
	}
	return (0);
}

t_dir	get_dir(t_proc *e, int *tab, int rotate)
{
	const int	raw = tab[0];
	const int	col = tab[1];
	const int	my_raw = (e->index / MAP_SIZE);
	const int	my_col = (e->index % MAP_SIZE);

	if (raw == -1)
		return (NOOPP);
	if (rotate)
	{
		if (raw < my_raw)
			return (TOP);
		else if (raw > my_raw)
			return (DOWN);
		else if (col < my_col)
			return (LEFT);
		return (RIGHT);
	}
	if (col < my_col)
		return (LEFT);
	else if (col > my_col)
		return (RIGHT);
	else if (raw < my_raw)
		return (TOP);
	return (DOWN);
}

void	move_player(t_dir dir, t_proc *e)
{
	if (g_tab[dir](e) == -1)
	{
		if (dir == TOP)
		{
			if (g_tab[RIGHT](e) == -1)
				g_tab[LEFT](e);
		}
		else if (dir == DOWN)
		{
			if (g_tab[LEFT](e) == -1)
				g_tab[RIGHT](e);
		}
		else if (dir == RIGHT)
		{
			if (g_tab[TOP](e) == -1)
				g_tab[DOWN](e);
		}
		else
		{
			if (g_tab[DOWN](e) == -1)
				g_tab[TOP](e);
		}
	}
}
