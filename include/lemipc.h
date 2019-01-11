/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:22:22 by bhamidi           #+#    #+#             */
/*   Updated: 2019/01/11 16:01:57 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <signal.h>

# define MAP_SIZE	30
# define SHM_NAME	"/lemipc"
# define TIME		500000
# define CLASSIC	0
# define ZOMBIE		1
# define MODE		ZOMBIE

typedef struct s_proc	t_proc;

typedef enum	e_dir
{
	RIGHT = 0,
	DOWN,
	LEFT,
	TOP,
	NOOPP
}				t_dir;

struct	s_proc
{
	void	*ptr;
	sem_t	*sem;
	key_t	key;
	int		msqid;
	int		number;
	int		index;
};

struct	s_msgbuf
{
	long	mtype;
	char	mtext[200];
};

int		get_ally(const char *str, int number);
int		send_position(t_proc *e, int *opp, struct s_msgbuf *buf);
t_dir	get_dir(t_proc *e, int *tab, int rotate);
void	move_player(t_dir dir, t_proc *e);
int		get_number(const char *str);
void	run(int fd);
void	game(int number, int fd);
void	init(void);
void	clean(void);
void	print(char *str);
void	play(t_proc *e, int mode);
int		can_play(t_proc *e, int mode);
int		move_top(t_proc *e);
int		move_right(t_proc *e);
int		move_down(t_proc *e);
int		move_left(t_proc *e);
void	loop(void *ptr, int *msqid, int key);
void	clean_prog(int sig);
int		retry(void);
int		nteam(const char *str);
void	send_finish(int msqid, const char *str);
int		get_nplayer(const char *str);
void	find_opponent(t_proc *e, int *opp);
int		opponent_pos(int number, const char *str);
void	delete_player(t_proc *e);

#endif
