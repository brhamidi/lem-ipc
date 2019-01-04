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
	if (MAP_SIZE > 50 ||((number = get_number(av[1])) == -1))
	{
		dprintf(2, "Error: bad team number\n");
		exit(EXIT_FAILURE);
	}
	if ((fd = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, 0644)) == -1)
	{
		if (errno == EEXIST)
		{
			printf("shm already open so just play\n");
			sleep(1);
			fd = shm_open(SHM_NAME, O_RDWR, 0644);
			if (fd == -1)
				perror("shm_open: ");
			else
				game(number, fd);
			return (0);
		}
		else
		{
			perror("shm_open: ");
			exit(EXIT_FAILURE);
		}
	}
	if (ftruncate(fd, (MAP_SIZE * MAP_SIZE)) == -1)
	{
		perror("ftruncate: ");
		shm_unlink(SHM_NAME);
		exit(EXIT_FAILURE);
	}
	if (sem_open(SHM_NAME, O_CREAT, 0644, 1) == SEM_FAILED)
		perror("sem_open: ");
	else
		run(fd);
	if (shm_unlink(SHM_NAME) == -1)
	{
		perror("shm_unlink: ");
		exit(EXIT_FAILURE);
	}
	return (0);
}
