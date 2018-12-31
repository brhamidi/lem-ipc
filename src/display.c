#include "lemipc.h"

void	init(void)
{
	initscr();
	noecho();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	cbreak();
	timeout(0);
}

void	print(char *str)
{
	int	i;
	int	raw;
	int	col;

	i = 0;
//	mvprintw(0, 0, "Use q to exit");
	while (i < MAP_SIZE * MAP_SIZE)
	{
		raw = i / MAP_SIZE;
		col = i % MAP_SIZE;
		if (str[i] == -1)
			mvprintw(2 + raw, col, ".");
		else
			mvprintw(2 + raw, col, "%d", str[i]);
		++i;
	}
}

void	clean(void)
{
	endwin();
}
