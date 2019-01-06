#include "lemipc.h"

int	blocked(int raw, int col, t_proc *e)
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
	return (1);
}

int	can_play(t_proc *e)
{
	int		amount;
	const int	raw = (e->index / MAP_SIZE);
	const int	col = (e->index % MAP_SIZE);

	amount = 0;
	amount += blocked(raw, col + 1, e);
	amount += blocked(raw, col - 1, e);
	amount += blocked(raw + 1, col, e);
	amount += blocked(raw - 1, col, e);
	amount += blocked(raw - 1, col - 1, e);
	amount += blocked(raw - 1, col + 1, e);
	amount += blocked(raw + 1, col + 1, e);
	amount += blocked(raw + 1, col - 1, e);
	return (amount > 1 ? 0 : 1);
}
