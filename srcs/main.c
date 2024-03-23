/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lboiteux <lboiteux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 23:20:49 by lboiteux          #+#    #+#             */
/*   Updated: 2024/03/19 23:20:49 by lboiteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	fork_lock(t_philo *philo)
{
	MUTEX_LOCK(philo->fork_left);
	print_message(philo, PHILO_FORK);
	if (philo->data.nb_philo == 1)
	{
		MUTEX_UNLOCK(philo->fork_left);
		sleep_philo(philo->data.time_die);
		return ;
	}
	MUTEX_LOCK(philo->fork_right);
	print_message(philo, PHILO_FORK);
	print_message(philo, PHILO_EATING);
	MUTEX_LOCK(philo->meal_mt);
	philo->data.last_meal = get_time();
	MUTEX_UNLOCK(philo->meal_mt);
	sleep_philo(philo->data.time_eat);
	MUTEX_UNLOCK(philo->fork_right);
	MUTEX_UNLOCK(philo->fork_left);
}

void	*routine(void *ph)
{
	t_philo	*philo;

	philo = (t_philo *)ph;
	if (philo->data.nb_philo % 2)
		usleep(100);
	while (1)
	{
		fork_lock(philo);
		if (philo->data.nb_philo == 1)
			break ;
		MUTEX_LOCK(philo->meal_mt);
		philo->data.nb_meal += 1;
		MUTEX_UNLOCK(philo->meal_mt);
		print_message(philo, PHILO_SLEEPING);
		sleep_philo(philo->data.time_sleep);
		print_message(philo, PHILO_THINKING);
		MUTEX_LOCK(philo->death_mt);
		if (*philo->death == 1)
		{
			MUTEX_UNLOCK(philo->death_mt);
			break ;
		}
		MUTEX_UNLOCK(philo->death_mt);
	}
	return (NULL);
}

void	end_check(t_philo **philo)
{
	int	i;
	int	died;

	died = 0;
	while (1)
	{
		i = -1;
		while (++i != (*philo)->data.nb_philo)
		{
			if ((!death_check(*philo) || !meal_check(philo)) && !died++)
			{
				MUTEX_LOCK((*philo)->death_mt);
				if (!*(*philo)->death)
					*(*philo)->death = 1;
				MUTEX_UNLOCK((*philo)->death_mt);
				break ;
			}
		}
		if (died)
			break ;
		usleep(100);
	}
}

void	start_simulation(t_philo **philo)
{
	pthread_t	*thread;
	int			i;

	get_time();
	thread = malloc((*philo)->data.nb_philo * sizeof(pthread_t));
	if (!thread)
		return ;
	memset(thread, 0, (*philo)->data.nb_philo * sizeof(pthread_t));
	i = -1;
	while (++i < (*philo)->data.nb_philo)
		pthread_create(&(thread[i]), NULL, routine, (void *)philo[i]);
	end_check(philo);
	i = -1;
	while (++i != (*philo)->data.nb_philo)
		pthread_join(thread[i], NULL);
	free(thread);
}

int	main(int ac, char **av)
{
	t_data			data;
	t_philo			**philo;

	if ((ac == 5) || (ac == 6))
	{
		data = fill_data(ac, av);
		if (data.nb_philo == -1 || data.time_die == -1 || data.time_eat \
		== -1 || data.time_sleep == -1 || data.nb_meal_max == -1)
		{
			printf(ERROR_MSG);
			return (0);
		}
		philo = malloc(data.nb_philo * sizeof(t_philo *));
		if (!philo)
			return (0);
		memset(philo, 0, data.nb_philo * sizeof(t_philo *));
		init_philo(philo, data);
	}
	else
	{
		printf(ERROR_MSG);
		return (0);
	}
	return (1);
}
