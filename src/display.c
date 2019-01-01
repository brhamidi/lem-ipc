#include "lemipc.h"

void	init(void)
{
	initscr();
	noecho();
	curs_set(FALSE);
	cbreak();
	timeout(0);
}

void	print(char *str)
{
	int	i;
	int	raw;
	int	col;

	i = 0;
	mvprintw(0, 0, "Use q to exit");
	while (i < MAP_SIZE * MAP_SIZE)
	{
		raw = i / MAP_SIZE;
		col = i % MAP_SIZE;
		if (str[i] == -1)
			mvprintw(2 + raw, col * 2, ".");
		else
			mvprintw(2 + raw, col * 2, "%d", str[i]);
		++i;
	}
}

void	clean(void)
{
	endwin();
}
