

#include "lemipc.h"


int	main(int ac, char **av)
{
	int	number;
	int	fd;

	if (ac != 2)
	{
		dprintf(2, "usage: %s TEAMNUMBER\n", av[0]);
		exit(EXIT_FAILURE);
	}
	if ((number = get_number(av[1])) == -1)
	{
		dprintf(2, "Error: bad team number\n");
		exit(EXIT_FAILURE);
	}
	if ((fd = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, 0644)) == -1)
	{
		if (errno == EEXIST)
		{
			printf("shm already open so just play\n");
			return (0);
		}
		else
		{
			perror("shm_open: ");
			exit(EXIT_FAILURE);
		}
	}
	if (ftruncate(fd, MAP_SIZE * MAP_SIZE) == -1)
	{
			perror("ftruncate: ");
			exit(EXIT_FAILURE);
	}
	print_map(fd);
	if (shm_unlink(SHM_NAME) == -1)
	{
		perror("shm_unlink: ");
		exit(EXIT_FAILURE);
	}
	return (0);
}
