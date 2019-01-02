#ifndef LEMIPC_H
# define LEMIPC_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <ctype.h>
# include <unistd.h>
# include <errno.h>
# include <sys/mman.h>
# include <sys/types.h>
# include <ncurses.h>
# include <string.h>
# include <time.h>
# include <semaphore.h>
# include <sys/ipc.h>
# include <sys/msg.h>

# define MAP_SIZE	20
# define SHM_NAME	"/lemipc"

typedef struct s_proc	t_proc;

struct s_proc
{
	void	*ptr;
	sem_t	*sem;
	key_t	key;
	int	msqid;
	int	number;
};

int	get_number(const char *str);
void	run(int fd);
void	game(int number, int fd);
void	init(void);
void	clean(void);
void	print(char *str);
void	play(t_proc *e);

#endif
