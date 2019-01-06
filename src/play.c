#include "lemipc.h"

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

void		play(t_proc *e)
{
	struct s_msgbuf	buf;
	t_dir		dir;

	buf.mtype = e->number;
	while (move_down(e))
	{
		if (msgrcv(e->msqid, &buf, sizeof(buf.mtext),
					e->number, IPC_NOWAIT) == -1)
		{
			if (errno != ENOMSG)
			{
				perror("msgrcv: ");
				return;
			}
			dir = find_opponent(e);
			buf.mtype = e->number;
			buf.mtext[0] = dir;
			if (msgsnd(e->msqid,
			    (void *) &buf, sizeof(buf.mtext), IPC_NOWAIT) == -1)
			{
				perror("msgsnd: ");
				return;
			}
			else
				printf("Dir sended: %d\n", dir);
		}
		else
		{
			dir = (int)buf.mtext[0];
			printf("Dir recvied: %d\n", dir);
		}
		if (dir == NOOPP)
			printf("info: No opponent\n");
		sleep(1);
	}
}
