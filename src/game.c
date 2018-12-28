#include "lemipc.h"

int	find_place(char *ptr, int number, int value, sem_t *sem)
{
	int	i;

	i = value;
	while (i < MAP_SIZE * MAP_SIZE)
	{
		if (ptr[i] == -1)
		{
			if (sem_wait(sem) == 1)
			{
				perror("sem_wait: ");
				return (1);
			}
			ptr[i] = number;
			if (sem_post(sem) == 1)
			{
				perror("sem_post: ");
				return (1);
			}
			return (0);
		}
		++i;
	}
	if (value != 0)
		return (find_place(ptr, number, 0, sem));
	return (1);
}

void	game(int number, int fd)
{
	void	*ptr;
	sem_t	*sem;

	printf("team %d playing\n", number);
	srand(time(NULL));
	if ((ptr = mmap(0, MAP_SIZE * MAP_SIZE, PROT_READ | PROT_WRITE,
					MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		perror("mmap: ");
		return;
	}
	sem = ptr + (MAP_SIZE * MAP_SIZE);
	if (sem_init(sem, 1, 1) == -1)
	{
		perror("sem_init: ");
		if (munmap(ptr, MAP_SIZE * MAP_SIZE))
			perror("mmap: ");
		return;
	}
	if (find_place((char *)ptr, number, rand() % (MAP_SIZE * MAP_SIZE), sem))
		printf("no place found so i cant play!");
	else
		printf("wip\n");
	if (sem_destroy(sem))
		perror("sem_destroy: ");
	if (munmap(ptr, MAP_SIZE * MAP_SIZE))
		perror("mmap: ");
}
