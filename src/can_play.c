#include "lemipc.h"

int	blocked(int index, t_proc *e)
{
	const char *str = (const char *)e->ptr;

	if (str[index] == -1)
		return (0);
	if (str[index] == e->number)
		return (0);
	return (1);
}

int	can_play(t_proc *e)
{
	int amount;
	
	amount = 0;
	if (e->index % MAP_SIZE > 0)
		amount += blocked(e->index - 1, e);
	if (e->index % MAP_SIZE < MAP_SIZE - 1)
		amount += blocked(e->index + 1, e);
	if (e->index / MAP_SIZE)
		amount += blocked(e->index - MAP_SIZE, e);
	if (e->index / MAP_SIZE < MAP_SIZE - 1)
		amount += blocked(e->index + MAP_SIZE, e);
	return (amount > 1 ? 0 : 1);
}
