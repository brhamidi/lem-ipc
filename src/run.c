#include "lemipc.h"

static void	print_map(char *ptr)
{
	int	i;

	i = 0;
	while (i < MAP_SIZE * MAP_SIZE)
	{
		if (i && i % MAP_SIZE == 0)
			printf("\n");
		if (ptr[i] == -1)
			printf("_ ");
		else
			printf("%d ", ptr[i]);
		++i;
	}
	printf("\n");
	printf("\n");
}

static void	loop(void *ptr)
{
	print_map((char *)ptr);
	sleep(1);
	loop(ptr);
}

void	run(int fd)
{
	void	*ptr;

	if ((ptr = mmap(0, MAP_SIZE * MAP_SIZE, PROT_READ | PROT_WRITE,
					MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		perror("mmap: ");
		return;
	}
	memset(ptr, -1, MAP_SIZE * MAP_SIZE);
	loop(ptr);
	if (munmap(ptr, MAP_SIZE * MAP_SIZE))
		perror("mmap: ");
}
