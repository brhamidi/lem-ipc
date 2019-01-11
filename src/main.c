/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:07:43 by bhamidi           #+#    #+#             */
/*   Updated: 2019/01/11 16:00:08 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void		delete_player(t_proc *e)
{
	char	*str;

	str = (char *)e->ptr;
	if (sem_wait(e->sem) == 1)
		return ;
	str[e->index] = -1;
	if (sem_post(e->sem) == 1)
		return ;
}

int			opponent_pos(int number, const char *str)
{
	int	i;

	i = 0;
	while (i < MAP_SIZE * MAP_SIZE)
	{
		if (str[i] != -1 && str[i] != number)
			return (i);
		++i;
	}
	return (-1);
}

void		find_opponent(t_proc *e, int *opp)
{
	const int	index_opp = opponent_pos(e->number, (const char *)e->ptr);
	const int	raw = (index_opp / MAP_SIZE);
	const int	col = (index_opp % MAP_SIZE);

	if (index_opp == -1)
	{
		opp[0] = -1;
		opp[1] = -1;
	}
	else
	{
		opp[0] = raw;
		opp[1] = col;
	}
}

static void	init_main(int *number, int ac, char **av)
{
	if (MODE != ZOMBIE && MODE != CLASSIC)
	{
		dprintf(2, "MODE macro must be ZOMBIE or CLASSIC\n");
		exit(EXIT_FAILURE);
	}
	if (ac != 2)
	{
		dprintf(2, "usage: %s TEAMNUMBER\n", av[0]);
		exit(EXIT_FAILURE);
	}
	if (MAP_SIZE > 75 || ((*number = get_number(av[1])) == -1))
	{
		dprintf(2, "Error: bad team number\n");
		exit(EXIT_FAILURE);
	}
}

int			main(int ac, char **av)
{
	int	number;
	int	fd;

	init_main(&number, ac, av);
	if ((fd = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, 0644)) == -1)
	{
		if (errno == EEXIST)
		{
			usleep(TIME);
			if ((fd = shm_open(SHM_NAME, O_RDWR, 0644)) != -1)
				game(number, fd);
			return (0);
		}
		exit(EXIT_FAILURE);
	}
	if (ftruncate(fd, (MAP_SIZE * MAP_SIZE)) == -1)
	{
		shm_unlink(SHM_NAME);
		exit(EXIT_FAILURE);
	}
	if (sem_open(SHM_NAME, O_CREAT, 0644, 1) != SEM_FAILED)
		run(fd);
	sem_unlink(SHM_NAME);
	shm_unlink(SHM_NAME);
	return (0);
}
