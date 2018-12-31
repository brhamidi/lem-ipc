#include "lemipc.h"

void	print_map(char *ptr)
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
	int	c;

	init();
	while (1)
	{
		c = getch();
		if (c == 'q')
			break;
		clear();
		print((char *)ptr);
		refresh();
		sleep(1);
	}
	clean();
}

void	run(int fd)
{
	void	*ptr;
//	key_t	key;
//	int	msqid;

	if ((ptr = mmap(0, MAP_SIZE * MAP_SIZE, PROT_READ | PROT_WRITE,
					MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		perror("mmap: ");
		return;
	}
	memset(ptr, -1, MAP_SIZE * MAP_SIZE);
//	system("touch msgq.txt");
//	if ((key = ftok("msgq.txt", 42)) == -1)
//		perror("ftok: ");
//	else
//	{
//		if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1)
//			perror("msgget: ");
//		else
			loop(ptr);
//	}
//	system("rm msgq.txt");
//	if (msgctl(msqid, IPC_RMID, NULL) == -1)
//		perror("msgctl: ");
	if (munmap(ptr, MAP_SIZE * MAP_SIZE))
		perror("mmap: ");
}
