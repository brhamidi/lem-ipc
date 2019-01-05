#include "lemipc.h"

static int	init_e(t_proc *e, int number, int fd)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	srand(time.tv_usec);
	if ((e->ptr = mmap(0, MAP_SIZE * MAP_SIZE, PROT_READ | PROT_WRITE,
					MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		perror("mmap: ");
		return 1;
	}
	if ((e->sem = sem_open(SHM_NAME, O_RDWR)) == SEM_FAILED)
	{
		perror("sem_open: ");
		if (munmap(e->ptr, MAP_SIZE * MAP_SIZE))
			perror("mmap: ");
		return 1;
	}
	e->number = number;
	if ((e->key = ftok("msgq.txt", 42)) == -1)
	{
		perror("ftok: ");
		if (sem_destroy(e->sem))
			perror("sem_destroy: ");
		if (munmap(e->ptr, MAP_SIZE * MAP_SIZE))
			perror("mmap: ");
		return (1);
	}
	if ((e->msqid = msgget(e->key, 0644)) == -1)
	{
		perror("msgget: ");
		if (sem_destroy(e->sem))
			perror("sem_destroy: ");
		if (munmap(e->ptr, MAP_SIZE * MAP_SIZE))
			perror("mmap: ");
		return (1);
	}
	return (0);
}

static void	close_e(t_proc *e)
{
	if (munmap(e->ptr, MAP_SIZE * MAP_SIZE))
		perror("mmap: ");
	if (sem_close(e->sem) == -1)
		perror("sem_close: ");
}

static int	find_place(char *ptr, int number, int value, sem_t *sem)
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
				return (-1);
			}
			ptr[i] = number;
			if (sem_post(sem) == 1)
			{
				perror("sem_post: ");
				return (-1);
			}
			return (i);
		}
		++i;
	}
	if (value != 0)
		return (find_place(ptr, number, 0, sem));
	return (-1);
}

void		game(int number, int fd)
{
	t_proc	e;

	if (init_e(&e, number, fd))
		return;
	e.index = find_place((char *)e.ptr, number,
			rand() % (MAP_SIZE * MAP_SIZE), e.sem);
	if (e.index == -1)
		dprintf(2, "Error: No place found so can t play!\n");
	else
		play(&e);
	close_e(&e);
}
