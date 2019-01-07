#include "lemipc.h"

int	(*tab[4])(t_proc *) =
{
	&move_right,
	&move_down,
	&move_left,
	&move_top
};

const char	*str_dir[4] =
{
	"RIGHT",
	"DOWN",
	"LEFT",
	"TOP"
};

void		delete_player(t_proc *e)
{
	char	*str;

	str = (char *)e->ptr;
	if (sem_wait(e->sem) == 1)
	{
		perror("sem_wait: ");
		return;
	}
	str[e->index] = -1;
	if (sem_post(e->sem) == 1)
	{
		perror("sem_post: ");
		return;
	}
}

static int	opponent_pos(int number, const char *str)
{
	int	i;

	i = 0;
	while (i < MAP_SIZE * MAP_SIZE)
	{
		if (str[i] != -1 && str[i] != number)
			return (i);
		++i;
	}
	return (-1);
}

static void	find_opponent(t_proc *e, int *opp)
{
	const int	index_opp = opponent_pos(e->number, (const char *)e->ptr);
	const int	raw = (index_opp / MAP_SIZE);
	const int	col = (index_opp % MAP_SIZE);

	if (index_opp == -1)
	{
		opp[0] = -1;
		opp[1] = -1;
	}
	else
	{
		opp[0] = raw;
		opp[1] = col;
	}
}

static int	get_ally(const char *str, int number)
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

static int	send_position(t_proc *e, int *opp, struct s_msgbuf *buf)
{
	int	n_ally;
	
	if ((n_ally  = get_ally((const char *)e->ptr, e->number)) == 0)
		return (0);
	while (n_ally)
	{
		buf->mtype = e->number;
		buf->mtext[0] = opp[0];
		buf->mtext[4] = opp[1];
		if (msgsnd(e->msqid, (void *) buf, sizeof(buf->mtext), IPC_NOWAIT) == -1)
		{
			perror("msgsnd: ");
			return (1);
		}
		--n_ally;
	}
	printf("Position sended r:%d c:%d\n", opp[0], opp[1]);
	return (0);
}

t_dir		get_dir(t_proc *e, int *tab, int rotate)
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
		else
			return (RIGHT);
	}
	if (col < my_col)
		return (LEFT);
	else if (col > my_col)
		return (RIGHT);
	else if (raw < my_raw)
		return (TOP);
	else
		return (DOWN);
}

void		play(t_proc *e)
{
	struct s_msgbuf	buf;
	t_dir		dir;
	int		opp[2];
	int		rotate;

	buf.mtype = e->number;
	opp[0] = -1;
	opp[1] = -1;
	dir = NOOPP;
	rotate = 1;
	while (can_play(e))
	{
		if (msgrcv(e->msqid, &buf, sizeof(buf.mtext), 4242, IPC_NOWAIT) != -1)
		{
			printf("Partir finish !\n");
			return;
		}
		if (msgrcv(e->msqid, &buf, sizeof(buf.mtext),
					e->number, IPC_NOWAIT) == -1)
		{
			if (errno != ENOMSG)
			{
				perror("msgrcv: ");
				return;
			}
			find_opponent(e, opp);
			if (send_position(e, opp, &buf))
				break;
			dir = get_dir(e, opp, rotate);
		}
		else
		{
			opp[0] = (int)buf.mtext[0];
			opp[1] = (int)buf.mtext[4];
			printf("Position recvied: %d and %d\n\n", opp[0], opp[1]);
			dir = get_dir(e, opp, rotate);
		}
		if (dir == NOOPP)
			printf("Info: No opponent\n");
		else
		{
			printf("Info: Opponent at: %d %d\n", opp[0], opp[1]);
			tab[dir](e);
		}
		usleep(TIME);
		rotate = (rotate) ? 0 : 1;
	}
	delete_player(e);
}
