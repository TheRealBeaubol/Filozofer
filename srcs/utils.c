/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lboiteux <lboiteux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 00:19:44 by lboiteux          #+#    #+#             */
/*   Updated: 2024/03/20 00:19:44 by lboiteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

long	get_time(void)
{
	struct timeval	t;
	static long		start_time;
	long			time;

	gettimeofday(&t, NULL);
	time = t.tv_sec * 1000 + t.tv_usec / 1000;
	if (start_time == 0)
		start_time = time;
	return (time - start_time);
}

int	meal_check(t_philo **philo)
{
	int	i;

	i = 0;
	MUTEX_LOCK((*philo)->meal_mt);
	while (i < (*philo)->data.nb_philo)
	{
		if ((philo[i])->data.nb_meal_max == -2)
		{
			MUTEX_UNLOCK((*philo)->meal_mt);
			return (1);
		}
		if ((philo[i])->data.nb_meal < (philo[i])->data.nb_meal_max)
		{
			MUTEX_UNLOCK((*philo)->meal_mt);
			return (1);
		}
		i++;
	}
	MUTEX_LOCK((*philo)->death_mt);
	*(*philo)->death = 1;
	MUTEX_UNLOCK((*philo)->death_mt);
	MUTEX_UNLOCK((*philo)->meal_mt);
	return (0);
}

int	death_check(t_philo *philo)
{
	MUTEX_LOCK(philo->meal_mt);
	if (get_time() - philo->data.last_meal >= philo->data.time_die)
	{
		MUTEX_UNLOCK(philo->meal_mt);
		print_message(philo, PHILO_DIED);
		MUTEX_LOCK(philo->death_mt);
		*philo->death = 1;
		MUTEX_UNLOCK(philo->death_mt);
		return (0);
	}
	MUTEX_UNLOCK(philo->meal_mt);
	MUTEX_LOCK(philo->death_mt);
	if (*philo->death == 0)
	{
		MUTEX_UNLOCK(philo->death_mt);
		return (1);
	}
	MUTEX_UNLOCK(philo->death_mt);
	return (0);
}

void	print_message(t_philo *philo, char *msg)
{
	MUTEX_LOCK(philo->death_mt);
	MUTEX_LOCK(philo->print_mt);
	if (!(*philo->death))
		printf(msg, get_time(), philo->id + 1);
	MUTEX_UNLOCK(philo->print_mt);
	MUTEX_UNLOCK(philo->death_mt);
}

void	sleep_philo(int type)
{
	long	start_time;

	start_time = get_time();
	while (get_time() - start_time < type)
		usleep(100);
}
