#include "lemipc.h"

static int	update(t_proc *e, int raw, int col, int old_i)
{
	const int	i = raw * MAP_SIZE + col;
	char		*str;

	str = (char *)e->ptr;
	if (str[i] != -1)
	{
		printf("Place don't available at index %d and raw %d col %d\n", i, raw, col);
		return ((str[i] == e->number) ? -1 : 0);
	}
	if (sem_wait(e->sem) == 1)
	{
		perror("sem_wait: ");
		return (0);
	}
	str[old_i] = -1;
	str[i] = e->number;
	e->index = i;
	if (sem_post(e->sem) == 1)
	{
		perror("sem_post: ");
		return (0);
	}
	return (1);
}

int		move_left(t_proc *e)
{
	const int	raw = (e->index / MAP_SIZE);
	const int	col = (e->index % MAP_SIZE);

	if (!col)
	{
		printf("can't move left\n");
		return (0);
	}
	return (update(e, raw, col - 1, e->index));
}

int		move_right(t_proc *e)
{
	const int	raw = (e->index / MAP_SIZE);
	const int	col = (e->index % MAP_SIZE);

	if (col == MAP_SIZE - 1)
	{
		printf("can't move right\n");
		return (0);
	}
	return (update(e, raw, col + 1, e->index));
}

int		move_down(t_proc *e)
{
	const int	raw = (e->index / MAP_SIZE);
	const int	col = (e->index % MAP_SIZE);

	if (raw == MAP_SIZE - 1)
	{
		printf("can't move down\n");
		return (0);
	}
	return (update(e, raw + 1, col, e->index));
}

int		move_top(t_proc *e)
{
	const int	raw = (e->index / MAP_SIZE);
	const int	col = (e->index % MAP_SIZE);

	if (raw == 0)
	{
		printf("can't move top\n");
		return (0);
	}
	return (update(e, raw - 1, col, e->index));
}
