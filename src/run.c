/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:24:36 by bhamidi           #+#    #+#             */
/*   Updated: 2019/01/11 16:04:51 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

t_proc g_clean;

static int	print_winner(const char *str, void *ptr, int *msqid, int key)
{
	int	i;
	int	number;
	int	x;
	int	y;

	i = -1;
	number = 0;
	while (++i < MAP_SIZE * MAP_SIZE)
		if (str[i] != -1)
		{
			number = str[i];
			break ;
		}
	send_finish(*msqid, ptr);
	msgctl(*msqid, IPC_RMID, NULL);
	usleep(1000);
	*msqid = msgget(key, 0644 | IPC_CREAT);
	clear();
	getmaxyx(stdscr, y, x);
	mvprintw(y / 2, x / 2 - 4, "GAME END");
	mvprintw(y / 2 + 2, x / 2 - 9, "Team winner is %d !", number);
	mvprintw(y / 2 + 5, x / 2 - 15, "Do you want to continue ? (y / n)");
	refresh();
	usleep(TIME);
	return (retry());
}

void		party_finish(int *msqid, void *ptr)
{
	send_finish(*msqid, ptr);
	usleep(TIME);
	clean();
}

void		retry_game(void *ptr, int *msqid, int key)
{
	clean();
	loop(ptr, msqid, key);
}

void		loop(void *ptr, int *msqid, int key)
{
	int	c;
	int	game;

	game = 0;
	init();
	while (1)
	{
		while ((c = getch()) != ERR)
			if (c == 'q')
				return (party_finish(msqid, ptr));
		print((char *)ptr);
		refresh();
		if (nteam((const char *)ptr) > 1 && !game)
			game = 1;
		if (nteam((const char *)ptr) == 1 && game)
		{
			if (print_winner((const char *)ptr, ptr, msqid, key))
				return (retry_game(ptr, msqid, key));
			break ;
		}
		usleep(TIME);
	}
	clean();
}

void		run(int fd)
{
	void	*ptr;
	key_t	key;
	int		msqid;

	if ((ptr = mmap(0, MAP_SIZE * MAP_SIZE, PROT_READ | PROT_WRITE,
					MAP_SHARED, fd, 0)) == MAP_FAILED)
		return ;
	memset(ptr, -1, MAP_SIZE * MAP_SIZE);
	system("touch msgq.txt");
	if ((key = ftok("msgq.txt", 42)) != -1)
	{
		if ((msqid = msgget(key, 0644 | IPC_CREAT)) != -1)
		{
			g_clean.ptr = ptr;
			g_clean.key = key;
			g_clean.msqid = msqid;
			signal(SIGINT, clean_prog);
			loop(ptr, &msqid, key);
			msgctl(msqid, IPC_RMID, NULL);
		}
	}
	system("rm msgq.txt");
	munmap(ptr, MAP_SIZE * MAP_SIZE);
}
