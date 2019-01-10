/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_number.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:07:11 by bhamidi           #+#    #+#             */
/*   Updated: 2019/01/10 20:07:26 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

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
