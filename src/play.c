#include "lemipc.h"

void	play(t_proc *e)
{
	while (move_top(e))
	{
		printf("Move sucess\n");
		sleep(1);
	}
}
