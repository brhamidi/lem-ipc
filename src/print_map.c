#include "lemipc.h"

void	print_tab(char *ptr)
{
	int	i;

	i = 0;
	while (i < MAP_SIZE * MAP_SIZE)
	{
		if (i && i % MAP_SIZE == 0)
			printf("\n");
		printf("%c ", ptr[i]);
		++i;
	}
	printf("\n");
}

void	print_map(int fd)
{
	void	*ptr;

	printf("%d\n", MAP_SIZE);
	if ((ptr = mmap(0, MAP_SIZE * MAP_SIZE, PROT_READ | PROT_WRITE,
					MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		perror("mmap: ");
		return;
	}
	memset(ptr, '0', MAP_SIZE * MAP_SIZE);
	print_tab((char *)ptr);
	if (munmap(ptr, MAP_SIZE * MAP_SIZE))
		perror("mmap: ");
}
