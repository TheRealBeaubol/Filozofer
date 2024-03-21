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

int	death_check(t_philo *philo)
{
	pthread_mutex_lock(philo->death_mt);
	if (*philo->death == 0)
	{
		pthread_mutex_unlock(philo->death_mt);
		return (1);
	}
	pthread_mutex_unlock(philo->death_mt);
	return (0);
}

int	meal_check(t_philo *philo)
{
	int	i;

	i = 0;
	while (i != philo->data.nb_philo)
	{
		if (philo[i].data.nb_meal > philo[i].data.nb_meal_max)
			return (0);
		i++;
	}
	return (1);
}

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

void	print_message(t_philo *philo, char *msg)
{
	pthread_mutex_lock(philo->print_mt);
	printf(msg, get_time(), philo->id + 1);
	pthread_mutex_unlock(philo->print_mt);
}

void	fork_lock(t_philo *philo, int *end)
{
	pthread_mutex_lock(philo->fork_right);
	if (!death_check(philo) || !meal_check(philo))
	{
		pthread_mutex_unlock(philo->fork_right);
		*end = 1;
		return ;
	}
	print_message(philo, PHILO_FORK);
	pthread_mutex_lock(philo->fork_left);
	if (!death_check(philo) || !meal_check(philo))
	{
		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);
		*end = 1;
		return ;
	}
	print_message(philo, PHILO_FORK);
	print_message(philo, PHILO_EATING);
	usleep(philo->data.time_eat);
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
	return ;
}

void	*routine(void *ph)
{
	t_philo	*philo;

	philo = (t_philo *)ph;
	while (*philo->end == 0)
	{
		if (philo-> id % 2)
			usleep(1000);
		fork_lock(philo, philo->end);
		pthread_mutex_lock(philo->meal_mt);
		philo->data.nb_meal += 1;
		pthread_mutex_unlock(philo->meal_mt);
		print_message(philo, PHILO_SLEEPING);
		usleep(philo->data.time_sleep);
		print_message(philo, PHILO_THINKING);
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	int				i;
	int				death;
	int				end;
	int				nb_philo;
	t_data			data;
	t_philo			**philo;
	pthread_t		*thread;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mt;
	pthread_mutex_t	meal_mt;
	pthread_mutex_t	death_mt;

	if (ac > 6)
		return (0);
	nb_philo = ft_atoi(av[1]);
	data = (t_data){nb_philo, 0, 2, 1000, 500, 500};
	philo = malloc(nb_philo * sizeof(t_philo *));
	forks = malloc(nb_philo * sizeof(pthread_mutex_t));
	thread = malloc(nb_philo * sizeof(pthread_t));
	memset((void *) philo, 0, nb_philo * sizeof(t_philo *));
	memset((void *) thread, 0, nb_philo * sizeof(pthread_t));
	memset((void *) &print_mt, 0, sizeof(pthread_mutex_t));
	memset((void *) &meal_mt, 0, sizeof(pthread_mutex_t));
	memset((void *) &death_mt, 0, sizeof(pthread_mutex_t));
	memset((void *) forks, 0, nb_philo * sizeof(pthread_mutex_t));
	i = 0;
	death = 0;
	end = 0;
	while (i < nb_philo)
	{
		philo[i] = malloc(sizeof(t_philo));
		if (!philo[i])
			return (0);
		philo[i]->id = i;
		philo[i]->death = &death;
		philo[i]->end = &end;
		philo[i]->data = data;
		if (i != nb_philo - 1)
		{
			philo[i]->fork_right = &forks[i];
			philo[i]->fork_left = &forks[(i + 1) % nb_philo];
		}
		else
		{
			philo[i]->fork_left = &forks[i];
			philo[i]->fork_right = &forks[(i + 1) % nb_philo];
		}
		philo[i]->meal_mt = &meal_mt;
		philo[i]->print_mt = &print_mt;
		philo[i]->death_mt = &death_mt;
		pthread_create(&(thread[i]), NULL, routine, (void *)philo[i]);
		i++;
	}
	i = -1;
	while (++i != nb_philo)
		pthread_join(thread[i], NULL);
	return (1);
}
