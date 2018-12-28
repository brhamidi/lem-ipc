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
# include <ncurses.h>
# include <string.h>

# define MAP_SIZE	15
# define SHM_NAME	"/lemipc"

int	get_number(const char *str);
void	print_map(char *str);
void	run(int fd);

#endif
