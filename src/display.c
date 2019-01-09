#include "lemipc.h"

void	init(void)
{
	initscr();
	noecho();
	curs_set(FALSE);
	cbreak();
	timeout(0);
	start_color();
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	init_pair(6, COLOR_WHITE, COLOR_BLACK);
	init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
}

void	print(char *str)
{
	int	i;
	int	raw;
	int	col;

	i = 0;
	mvprintw(0, 0, "Use q to exit");
	mvwhline(stdscr, 1, 1, '_', MAP_SIZE * 2 + 1);
	mvwhline(stdscr, MAP_SIZE + 2, 0, '_', MAP_SIZE * 2 + 2);
	mvwvline(stdscr, 2, 0, '|', MAP_SIZE + 1);
	mvwvline(stdscr, 2, MAP_SIZE * 2 + 2 , '|', MAP_SIZE + 1);
	while (i < MAP_SIZE * MAP_SIZE)
	{
		raw = i / MAP_SIZE;
		col = i % MAP_SIZE;
		if (str[i] == -1)
			mvprintw(2 + raw, col * 2 + 2, ".");
		else
		{
			attron(COLOR_PAIR(str[i] % 7 + 1));
			mvprintw(2 + raw, col * 2 + 2, "%d", str[i]);
			attroff(COLOR_PAIR(str[i] % 7 + 1));
		}
		++i;
	}
}

void	clean(void)
{
	endwin();
}
