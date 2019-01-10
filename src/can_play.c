/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   can_play.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:02:02 by bhamidi           #+#    #+#             */
/*   Updated: 2019/01/10 20:03:02 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

static int	blocked(int raw, int col, t_proc *e)
{
	const char	*str = (const char *)e->ptr;
	const int	i = raw * MAP_SIZE + col;

	if (col < 0 || col > MAP_SIZE - 1)
		return (0);
	if (raw < 0 || raw > MAP_SIZE - 1)
		return (0);
	if (str[i] == -1)
		return (0);
	if (str[i] == e->number)
		return (0);
	return (str[i]);
}

static int	doublon(const int *tab, int value, int index, int sum)
{
	if (sum >= 2)
		return (1);
	if (index == 8)
		return (0);
	if (tab[index] == value)
		return (doublon(tab, value, index + 1, sum + 1));
	return (doublon(tab, value, index + 1, sum));
}

static int	verif(const int *tab, int index)
{
	if (index == 8)
		return (0);
	if (tab[index] != 0)
		if (doublon(tab, tab[index], 0, 0))
			return (tab[index]);
	return (verif(tab, index + 1));
}

static void	update_player(t_proc *e)
{
	char	*str;

	str = (char *)e->ptr;
	if (sem_wait(e->sem) == 1)
		return ;
	str[e->index] = e->number;
	if (sem_post(e->sem) == 1)
		return ;
}

int			can_play(t_proc *e, int mode)
{
	int			tab[8];
	const int	raw = (e->index / MAP_SIZE);
	const int	col = (e->index % MAP_SIZE);
	int			value;

	tab[0] = blocked(raw, col + 1, e);
	tab[1] = blocked(raw, col - 1, e);
	tab[2] = blocked(raw + 1, col, e);
	tab[3] = blocked(raw - 1, col, e);
	tab[4] = blocked(raw - 1, col - 1, e);
	tab[5] = blocked(raw - 1, col + 1, e);
	tab[6] = blocked(raw + 1, col + 1, e);
	tab[7] = blocked(raw + 1, col - 1, e);
	value = verif(tab, 0);
	if (mode == 1 && value > 0)
	{
		e->number = value;
		update_player(e);
		return (1);
	}
	return (value > 0 ? 0 : 1);
}
