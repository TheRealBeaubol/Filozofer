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

int	is_all_feed(t_philo *philo)
{
	int	i;

	i = 0;
	while (i != philo->data.nb_philo)
	{
		if (philo[i].data.nb_meal < philo[i].data.nb_meal_max)
			return (0);
		i++;
	}
	return (1);
}

int	check_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork_left->fork);
	if (philo->fork_left->used == 1)
	{	
		pthread_mutex_unlock(&philo->fork_left->fork);
		return (1);
	}
	pthread_mutex_lock(&philo->fork_right->fork);
	if (philo->fork_right->used == 1 || philo->fork_left->used == 1)
	{
		pthread_mutex_unlock(&philo->fork_right->fork);	
		pthread_mutex_unlock(&philo->fork_left->fork);
		return (1);
	}
	if (philo->fork_right->used == 0 && philo->fork_left->used == 0)
	{
		philo->fork_left->used = 1;
		philo->fork_right->used = 1;
		pthread_mutex_unlock(&philo->fork_right->fork);	
		pthread_mutex_unlock(&philo->fork_left->fork);
		return (0);
	}
	pthread_mutex_unlock(&philo->fork_right->fork);
	pthread_mutex_unlock(&philo->fork_left->fork);
	return (0);
}

void	*filo_eating_fork(void *ph)
{
	t_philo	*philo;

	philo = (t_philo *)ph;
	while (!is_all_feed(philo))
	{
		while (check_fork(philo))
			usleep(1000);
		usleep(philo->data.time_eat);
		pthread_mutex_lock(&philo->fork_left->fork);
		philo->fork_left->used = 0;
		pthread_mutex_unlock(&philo->fork_left->fork);
		pthread_mutex_lock(&philo->fork_right->fork);
		philo->fork_right->used = 0;
		pthread_mutex_unlock(&philo->fork_right->fork);
		pthread_mutex_lock(philo->print);
		printf(PHILO_EATING, philo->id + 1);
		pthread_mutex_unlock(philo->print);
		philo->data.nb_meal += 1;
		usleep(philo->data.time_sleep);
		pthread_mutex_lock(philo->print);
		printf(PHILO_SLEEPING, philo->id + 1);
		pthread_mutex_unlock(philo->print);
		pthread_mutex_lock(philo->print);
		printf(PHILO_THINKING, philo->id + 1);
		pthread_mutex_unlock(philo->print);
	}
	return (NULL);
}

int	init_fork_right(int id, int id_max)
{
	if (id == id_max)
		return (0);
	return (id);
}

int	main(int ac, char **av)
{
	int				i;
	int				nb_philo;
	t_fork			*forks;
	t_data			data;
	t_philo			**philo;
	pthread_t		*thread;
	pthread_mutex_t	print;

	if (ac > 6)
		return (0);
	nb_philo = ft_atoi(av[1]);
	data = (t_data) {nb_philo, 0, 5, 400, 500000, 500000};
	philo = malloc(nb_philo * sizeof(t_philo *));
	thread = malloc(nb_philo * sizeof(pthread_t));
	forks = malloc(nb_philo * sizeof(t_fork));
	memset((void *) philo, 0, nb_philo * sizeof(t_philo));
	memset((void *) thread, 0, nb_philo * sizeof(pthread_t));
	memset((void *) forks, 0, nb_philo * sizeof(t_fork));
	memset((void *) &print, 0, sizeof(pthread_mutex_t));
	i = 0;
	while (i < nb_philo)
	{
		philo[i] = malloc(sizeof(t_philo));
		if (!philo[i])
			return (0);
		philo[i]->id = i;
		philo[i]->print = &print;
		philo[i]->data = data;
		philo[i]->fork_left = &forks[i];
		philo[i]->fork_right = &forks[(i + 1) % nb_philo];
		pthread_create(&(thread[i]), NULL, filo_eating_fork, (void *)philo[i]);
		i++;
	}
	i = -1;
	while (++i != nb_philo)
		pthread_join(thread[i], NULL);
	(void)av;
	return (1);
}
