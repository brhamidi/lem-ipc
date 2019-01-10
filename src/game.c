#include "lemipc.h"

static int	init_e(t_proc *e, int number, int fd)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	srand(time.tv_usec);
	if ((e->ptr = mmap(0, MAP_SIZE * MAP_SIZE, PROT_READ | PROT_WRITE,
					MAP_SHARED, fd, 0)) == MAP_FAILED)
		return (1);
	if ((e->sem = sem_open(SHM_NAME, O_RDWR)) == SEM_FAILED)
	{
		munmap(e->ptr, MAP_SIZE * MAP_SIZE);
		return (1);
	}
	e->number = number;
	if ((e->key = ftok("msgq.txt", 42)) == -1)
	{
		sem_unlink(SHM_NAME);
		munmap(e->ptr, MAP_SIZE * MAP_SIZE);
		return (1);
	}
	if ((e->msqid = msgget(e->key, 0644)) == -1)
	{
		sem_unlink(SHM_NAME);
		munmap(e->ptr, MAP_SIZE * MAP_SIZE);
		return (1);
	}
	return (0);
}

static void	close_e(t_proc *e)
{
	munmap(e->ptr, MAP_SIZE * MAP_SIZE);
	sem_close(e->sem);
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
				return (-1);
			ptr[i] = number;
			if (sem_post(sem) == 1)
				return (-1);
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
	if (e.index != -1)
		play(&e, ZOMBIE);
	close_e(&e);
}
