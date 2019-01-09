#include "lemipc.h"

void	init(void)
{
	const char	*str = "\
 _                      _            \n\
| | ___ _ __ ___       (_)_ __   ___ \n\
| |/ _ \\ '_ ` _ \\ _____| | '_ \\ / __|\n\
| |  __/ | | | | |_____| | |_) | (__ \n\
|_|\\___|_| |_| |_|     |_| .__/ \\___|\n\
                         |_|         ";

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
	mvprintw(6, 0, "Use q to exit");
	mvprintw(0, 0, str);
	mvwhline(stdscr, 1 + 6, 1, '_', MAP_SIZE * 3 + 1);
	mvwhline(stdscr, MAP_SIZE + 2 + 6, 0, '_', MAP_SIZE * 3 + 2);
	mvwvline(stdscr, 2 + 6, 0, '|', MAP_SIZE + 1);
	mvwvline(stdscr, 2 + 6, MAP_SIZE * 3 + 2 , '|', MAP_SIZE + 1);
}

void	print(char *str)
{
	int	i;
	int	raw;
	int	col;

	i = 0;
	while (i < MAP_SIZE * MAP_SIZE)
	{
		raw = i / MAP_SIZE;
		col = i % MAP_SIZE;
		if (str[i] == -1)
			mvprintw(2 + raw + 6, col * 3 + 2, " ");
		else
		{
			attron(COLOR_PAIR(str[i] % 7 + 1));
			mvprintw(2 + raw + 6, col * 3 + 2, "%d", str[i]);
			attroff(COLOR_PAIR(str[i] % 7 + 1));
		}
		++i;
	}
}

void	clean(void)
{
	endwin();
}
