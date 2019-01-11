/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_number.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:07:11 by bhamidi           #+#    #+#             */
/*   Updated: 2019/01/11 15:58:34 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

extern t_proc g_clean;

void		clean_prog(int sig)
{
	int	x;

	(void)sig;
	clear();
	getmaxyx(stdscr, x, x);
	mvprintw(2, x / 2 - 20, "SIGNAL INTERRUPT CATCHED");
	mvprintw(4, x / 2 - 20, "GAME WILL STOP ..");
	refresh();
	send_finish(g_clean.msqid, g_clean.ptr);
	usleep(TIME * 3);
	clean();
	msgctl(g_clean.msqid, IPC_RMID, NULL);
	system("rm msgq.txt");
	munmap(g_clean.ptr, MAP_SIZE * MAP_SIZE);
	sem_unlink(SHM_NAME);
	shm_unlink(SHM_NAME);
	exit(EXIT_FAILURE);
}

static int	ft_strmap(const char *str, int (*f)(int))
{
	if (!*str)
		return (0);
	if (!f(*str))
		return (1);
	return (ft_strmap(str + 1, f));
}

int			get_number(const char *str)
{
	int	number;

	if (ft_strmap(str, isdigit))
		return (-1);
	number = atoi(str);
	return (number >= MAP_SIZE || number < 0 ? -1 : number);
}
