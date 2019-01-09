#include "lemipc.h"

t_proc g_clean;

static int	get_nplayer(const char *str)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (i < MAP_SIZE * MAP_SIZE)
	{
		if (str[i] != -1)
			++result;
		++i;
	}
	return (result);
}

static void	send_finish(int msqid, const char *str)
{
	int		n;
	struct s_msgbuf	buf;
	
	n = get_nplayer(str);
	buf.mtype = 4242;
	while (n)
	{
		msgsnd(msqid, (void *) &buf, sizeof(buf.mtext), IPC_NOWAIT);
		--n;
	}
}

static void	loop(void *ptr, int msqid)
{
	int		c;

	init();
	while (1)
	{
		while ((c = getch()) != ERR)
			if (c == 'q')
			{
				send_finish(msqid, ptr);
				usleep(TIME * 2);
				clean();
				return;
			}
		clear();
		print((char *)ptr);
		refresh();
		usleep(TIME);
	}
	clean();
}

static void	clean_prog(int sig)
{
	int	x;

	(void)sig;
	clear();
	getmaxyx(stdscr, x, x);
	mvprintw(2, x / 2 - 20, "SIGNAL INTERRUPT CATCHED");
	mvprintw(4, x / 2 - 20, "GAME WILL STOP ..");
	refresh();
	send_finish(g_clean.msqid, g_clean.ptr);
	usleep(TIME * 4);
	clean();
	if (msgctl(g_clean.msqid, IPC_RMID, NULL) == -1)
		perror("msgctl: ");
	system("rm msgq.txt");
	if (munmap(g_clean.ptr, MAP_SIZE * MAP_SIZE))
		perror("mmap: ");
	if (sem_unlink(SHM_NAME) == -1)
		perror("sem_unlink: ");
	if (shm_unlink(SHM_NAME) == -1)
		perror("shm_unlink: ");
	exit(EXIT_FAILURE);
}

void		run(int fd)
{
	void	*ptr;
	key_t	key;
	int	msqid;

	if ((ptr = mmap(0, MAP_SIZE * MAP_SIZE, PROT_READ | PROT_WRITE,
					MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		perror("mmap: ");
		return;
	}
	memset(ptr, -1, MAP_SIZE * MAP_SIZE);
	system("touch msgq.txt");
	if ((key = ftok("msgq.txt", 42)) == -1)
		perror("ftok: ");
	else
	{
		if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1)
			perror("msgget: ");
		else
		{
			g_clean.ptr = ptr;
			g_clean.key = key;
			g_clean.msqid = msqid;
			signal(SIGINT, clean_prog);
			loop(ptr, msqid);
			if (msgctl(msqid, IPC_RMID, NULL) == -1)
				perror("msgctl: ");
		}
	}
	system("rm msgq.txt");
	if (munmap(ptr, MAP_SIZE * MAP_SIZE))
		perror("mmap: ");
}
