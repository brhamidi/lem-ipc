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
# include <sys/time.h>

# define MAP_SIZE	50
# define SHM_NAME	"/lemipc"

typedef struct s_proc	t_proc;

typedef enum	e_dir
{
	RIGHT = 0,
	DOWN,
	LEFT,
	TOP,
	NOOPP
} 		t_dir;

struct s_proc
{
	void	*ptr;
	sem_t	*sem;
	key_t	key;
	int	msqid;
	int	number;
	int	index;
};

struct	s_msgbuf
{
	long	mtype;
	char	mtext[1];
};

int	get_number(const char *str);
void	run(int fd);
void	game(int number, int fd);
void	init(void);
void	clean(void);
void	print(char *str);
void	play(t_proc *e);
int	can_play(t_proc *e);
int	move_top(t_proc *e);
int	move_right(t_proc *e);
int	move_down(t_proc *e);
int	move_left(t_proc *e);

#endif
