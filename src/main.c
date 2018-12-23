

#include "lemipc.h"

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		dprintf(2, "usage: %s TEAMNUMBER\n", av[0]);
		exit(EXIT_FAILURE);
	}
	return (0);
}
