/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:14:40 by bhamidi           #+#    #+#             */
/*   Updated: 2019/01/11 16:04:00 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

int	(*g_tab[4])(t_proc *) =
{
	&move_right,
	&move_down,
	&move_left,
	&move_top
};

static void	init_play(int *opp, int *rotate, struct s_msgbuf *buf, t_proc *e)
{
	buf->mtype = e->number;
	opp[0] = -1;
	opp[1] = -1;
	*rotate = 1;
}

t_dir		find_and_search_opp(t_proc *e, int *opp, struct s_msgbuf *buf,
		int rotate)
{
	find_opponent(e, opp);
	send_position(e, opp, buf);
	return (get_dir(e, opp, rotate));
}

int			end_play(t_dir dir, t_proc *e, int rotate)
{
	if (dir != NOOPP)
		move_player(dir, e);
	usleep(TIME);
	return (rotate ? 0 : 1);
}

t_dir		get_dir_opp(int *opp, struct s_msgbuf *buf, t_proc *e, int rotate)
{
	opp[0] = (int)buf->mtext[0];
	opp[1] = (int)buf->mtext[4];
	return (get_dir(e, opp, rotate));
}

void		play(t_proc *e, int mode)
{
	struct s_msgbuf	buf;
	t_dir			dir;
	int				opp[2];
	int				rotate;

	init_play(opp, &rotate, &buf, e);
	while (can_play(e, mode))
	{
		if (msgrcv(e->msqid, &buf, sizeof(buf.mtext), 4242, IPC_NOWAIT) != -1)
			break ;
		if (msgrcv(e->msqid, &buf, sizeof(buf.mtext),
					e->number, IPC_NOWAIT) == -1)
		{
			if (errno != ENOMSG)
				break ;
			dir = find_and_search_opp(e, opp, &buf, rotate);
		}
		else
			dir = get_dir_opp(opp, &buf, e, rotate);
		rotate = end_play(dir, e, rotate);
	}
	usleep(TIME);
	delete_player(e);
}
