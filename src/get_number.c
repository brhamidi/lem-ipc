

#include "lemipc.h"


int	ft_strmap(const char *str, int (*f)(int))
{
	if (!*str)
		return (0);
	if (!f(*str))
		return (1);
	return (ft_strmap(str + 1, f));
}

int	get_number(const char *str)
{
	int	number;

	if (ft_strmap(str, isdigit))
		return (-1);
	number = atoi(str);
	return (number >= MAP_SIZE || number < 0 ? -1: number);
}
