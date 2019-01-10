#include "lemipc.h"

int	main(int ac, char **av)
{
	int	number;
	int	fd;
	pid_t	pid;

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
			usleep(TIME);
			if ((fd = shm_open(SHM_NAME, O_RDWR, 0644)) != -1)
				game(number, fd);
			return (0);
		}
		else
			exit(EXIT_FAILURE);
	}
	if (ftruncate(fd, (MAP_SIZE * MAP_SIZE)) == -1)
	{
		shm_unlink(SHM_NAME);
		exit(EXIT_FAILURE);
	}
	if (sem_open(SHM_NAME, O_CREAT, 0644, 1) != SEM_FAILED)
	{
		pid = fork();
		if (pid == 0)
		{
			usleep(TIME);
			game(number, fd);
			exit(EXIT_SUCCESS);
		}
		else
			run(fd);

	}
	sem_unlink(SHM_NAME);
	shm_unlink(SHM_NAME);
	return (0);
}
