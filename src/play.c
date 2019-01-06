#include "lemipc.h"

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

static t_dir	find_opponent(t_proc *e)
{
	const int	index_opp = opponent_pos(e->number, (const char *)e->ptr);
	const int	raw = (index_opp / MAP_SIZE);
	const int	col = (index_opp % MAP_SIZE);
	const int	my_raw = (e->index / MAP_SIZE);
	const int	my_col = (e->index % MAP_SIZE);

	if (index_opp == -1)
		return (NOOPP);
	if (raw < my_raw)
		return (TOP);
	else if (raw > my_raw)
		return (DOWN);
	else if (col < my_col)
		return (LEFT);
	else
		return (RIGHT);
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

static int	send_dir(t_proc *e, t_dir dir, struct s_msgbuf *buf)
{
	int	n_ally;
	
	if ((n_ally  = get_ally((const char *)e->ptr, e->number)) == 0)
	{
		printf("I m alone ..\n");
		return (0);
	}
	while (n_ally)
	{
		buf->mtype = e->number;
		buf->mtext[0] = dir;
		if (msgsnd(e->msqid, (void *) buf, sizeof(buf->mtext), IPC_NOWAIT) == -1)
		{
			perror("msgsnd: ");
			return (1);
		}
		--n_ally;
	}
	return (0);
}

int	(*tab[4])(t_proc *) =
{
	&move_right,
	&move_down,
	&move_left,
	&move_right
};

void		play(t_proc *e)
{
	struct s_msgbuf	buf;
	t_dir		dir;

	buf.mtype = e->number;
	while (can_play(e))
	{
		if (msgrcv(e->msqid, &buf, sizeof(buf.mtext),
					4242, IPC_NOWAIT) != -1)
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
			dir = find_opponent(e);
			if (send_dir(e, dir, &buf))
				break;
			printf("Dir sended: %d\n", dir);
		}
		else
		{
			dir = (int)buf.mtext[0];
			printf("Dir recvied: %d\n\n", dir);
		}
		if (dir == NOOPP)
			printf("info: No opponent\n");
		else
			tab[dir](e);
		sleep(1);
	}
	delete_player(e);
}
