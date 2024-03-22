/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lboiteux <lboiteux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:04:15 by lboiteux          #+#    #+#             */
/*   Updated: 2024/03/22 16:03:36 by lboiteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	isdigit(int c)
{
	return (c <= '9' && c >= '0');
}

int	philo_cmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != 0 && s2[i] != 0 && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	philo_atoi(char *nptr)
{
	int	i;
	int	stock;

	i = 0;
	stock = 0;
	if (!philo_cmp(nptr, "2147483647"))
		return (2147483647);
	while (nptr[i] != '\0' && isdigit(nptr[i]))
		stock = (stock * 10) + (nptr[i++] - 48);
	if (!isdigit(nptr[i]) && nptr[i] != '\0')
		return (-1);
	return (stock);
}

t_data	fill_data(int ac, char **av)
{
	t_data	data;

	data.nb_philo = philo_atoi(av[1]);
	data.time_die = philo_atoi(av[2]);
	data.time_eat = philo_atoi(av[3]);
	data.time_sleep = philo_atoi(av[4]);
	if (ac == 6)
		data.nb_meal_max = philo_atoi(av[5]);
	data.nb_meal = 0;
	data.last_meal = 0;
	return (data);
}
